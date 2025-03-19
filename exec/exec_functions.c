/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:52:16 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/19 14:40:45 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Compte le nombre de commandes dans la liste chaînée
int	count_commands(t_command *cmd)
{
	int			count;
	t_command	*tmp;

	count = 0;
	tmp = cmd;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

// Gère l'exécution d'une commande dans un pipeline
static int	setup_pipe(t_command *cmd, t_pipeline_data *pipeline)
{
	if (cmd->next && pipe(pipeline->pipe_fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		return (1); // Erreur
	}
	return (0); // Succès
}

static void	execute_command(t_exec_data *exec_data, t_command *cmd)
{
	int		status;
	char	*path;

	if (is_builtin(cmd->args[0], exec_data->builtins))
	{
		status = execute_builtin(exec_data->shell, cmd, exec_data->builtins);
		exit(status);
	}
	path = find_path(cmd->args[0], exec_data->shell->env);
	if (!path || access(path, X_OK) != 0)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd("\n", 2);
		free(path);
		exit(127);
	}
	execve(path, cmd->args, env_to_array(exec_data->shell->env));
	free(path);
	exit(1);
}

// Fork et exécute la commande
static void	fork_and_execute(t_exec_data *exec_data, t_command *cmd,
		int heredoc_fd)
{
	exec_data->pipeline->pids[exec_data->index] = fork();
	if (exec_data->pipeline->pids[exec_data->index] == 0)
	{
		setup_fds_for_command(exec_data->pipeline->prev_fd, heredoc_fd,
			exec_data->pipeline->pipe_fd, cmd);
		handle_redirection(cmd, -1, exec_data->shell);
		execute_command(exec_data, cmd);
	}
}

// Ferme les descripteurs de fichiers dans le parent
static void	cleanup_fds(t_exec_data *exec_data, t_command *cmd, int heredoc_fd)
{
	if (heredoc_fd != -1)
		close(heredoc_fd);
	if (exec_data->pipeline->prev_fd != STDIN_FILENO)
		close(exec_data->pipeline->prev_fd);
	if (cmd->next)
	{
		close(exec_data->pipeline->pipe_fd[1]);
		exec_data->pipeline->prev_fd = exec_data->pipeline->pipe_fd[0];
	}
}

// Gère l'exécution d'une commande dans un pipeline
void	handle_command_execution(t_command *cmd, t_exec_data *exec_data)
{
	int	heredoc_fd;

	heredoc_fd = handle_heredoc_for_pipeline(exec_data->shell, cmd);
	if (setup_pipe(cmd, exec_data->pipeline))
	{
		exec_data->shell->exit_status = 1;
		return ;
	}
	fork_and_execute(exec_data, cmd, heredoc_fd);
	cleanup_fds(exec_data, cmd, heredoc_fd);
}

int	execute_builtin(t_shell *shell, t_command *cmd, t_builtin *builtins)
{
	int	i;

	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd->args[0], builtins[i].name) == 0)
		{
			if (ft_strcmp(cmd->args[0], "export") == 0)
				return (ft_export(cmd->args, shell));
			if (ft_strcmp(cmd->args[0], "unset") == 0)
				return (ft_unset(cmd->args, shell));
			if (ft_strcmp(cmd->args[0], "env") == 0)
				return (builtins[i].func(env_to_array(shell->env), shell));
			return (builtins[i].func(cmd->args, shell));
		}
		i++;
	}
	return (-1);
}

static void	execute_external_command(t_shell *shell, t_command *cmd, t_env *env)
{
	char	*path;

	(void)shell;
	path = find_path(cmd->args[0], env);
	if (!path || access(path, X_OK) != 0)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd("\n", 2);
		free(path);
		exit(127);
	}
	execve(path, cmd->args, env_to_array(env));
	free(path);
	exit(EXIT_FAILURE);
}

// Gère le fork pour la commande externe
static void	fork_external_command(t_shell *shell, t_command *cmd, t_env *env,
		int heredoc_fd)
{
	pid_t	pid;

	g_shell_state = 1;
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("Fork failed\n", 2);
		shell->exit_status = 1;
		g_shell_state = 0;
		return ;
	}
	if (pid == 0)
	{
		handle_redirection(cmd, heredoc_fd, shell);
		execute_external_command(shell, cmd, env);
	}
	if (heredoc_fd != -1)
		close(heredoc_fd);
	waitpid(pid, &shell->exit_status, 0);
	if (WIFEXITED(shell->exit_status))
		shell->exit_status = WEXITSTATUS(shell->exit_status);
	g_shell_state = 0;
}

// Fonction principale
void	execute_external(t_shell *shell, t_command *cmd, t_env *env,
		int heredoc_fd)
{
	fork_external_command(shell, cmd, env, heredoc_fd);
}

int	handle_heredoc_for_pipeline(t_shell *shell, t_command *cmd)
{
	if (cmd->heredoc)
		return (handle_heredoc(shell, cmd));
	return (-1);
}

void	setup_fds_for_command(int prev_fd, int heredoc_fd, int *pipe_fd,
		t_command *cmd)
{
	if (prev_fd != STDIN_FILENO)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (heredoc_fd != -1)
	{
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

void	execute_single_command(t_shell *shell, t_command *cmd,
		t_builtin *builtins)
{
	int		status;
	char	*path;

	if (is_builtin(cmd->args[0], builtins))
	{
		status = execute_builtin(shell, cmd, builtins);
		exit(status);
	}
	path = find_path(cmd->args[0], shell->env);
	if (!path || access(path, X_OK) != 0)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd("\n", 2);
		free(path);
		exit(127);
	}
	execve(path, cmd->args, env_to_array(shell->env));
	free(path);
	exit(1);
}

// Gère l'exécution d'un pipeline
static void	init_pipeline_data(t_shell *shell, t_pipeline_data *pipeline,
		int *cmd_count)
{
	t_command	*cmd;

	cmd = shell->cmds;
	*cmd_count = count_commands(cmd);
	pipeline->prev_fd = STDIN_FILENO;
	pipeline->pids = malloc(sizeof(pid_t) * (*cmd_count));
	if (!pipeline->pids)
	{
		ft_putstr_fd("Memory allocation failed\n", 2);
		exit(1);
	}
}

// Exécute les commandes du pipeline
static void	execute_pipeline_commands(t_shell *shell, t_builtin *builtins)
{
	t_command	*cmd;
	int			i;
	t_exec_data	exec_data;

	exec_data.shell = shell;
	exec_data.builtins = builtins;
	cmd = shell->cmds;
	i = 0;
	while (cmd)
	{
		exec_data.index = i;
		handle_command_execution(cmd, &exec_data);
		cmd = cmd->next;
		i++;
	}
}

// Attend les processus du pipeline
static void	wait_for_pipeline(t_shell *shell, t_pipeline_data *pipeline,
		int cmd_count)
{
	int	i;

	i = cmd_count;
	while (i-- > 0)
	{
		waitpid(pipeline->pids[i], &shell->exit_status, 0);
		if (WIFEXITED(shell->exit_status))
			shell->exit_status = WEXITSTATUS(shell->exit_status);
	}
	free(pipeline->pids);
}

// Fonction principale
void	execute_pipeline(t_shell *shell, t_builtin *builtins)
{
	t_pipeline_data	pipeline;
	int				cmd_count;

	init_pipeline_data(shell, &pipeline, &cmd_count);
	g_shell_state = 1;
	execute_pipeline_commands(shell, builtins);
	wait_for_pipeline(shell, &pipeline, cmd_count);
	g_shell_state = 0;
}

// Exécute une commande simple (non pipeline)
void	execute_single_cmd(t_shell *shell, t_command *cmd, t_builtin *builtins)
{
	int	saved_stdin;
	int	saved_stdout;
	int	heredoc_fd;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	heredoc_fd = handle_heredoc_for_pipeline(shell, cmd);
	if (is_builtin(cmd->args[0], builtins))
	{
		if (heredoc_fd != -1)
		{
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		handle_redirection(cmd, -1, shell);
		shell->exit_status = execute_builtin(shell, cmd, builtins);
	}
	else
		execute_external(shell, cmd, shell->env, heredoc_fd);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

// Décide comment exécuter les commandes
void	execute_commands(t_shell *shell, t_builtin *builtins)
{
	t_command	*cmd;

	cmd = shell->cmds;
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (cmd->next)
		execute_pipeline(shell, builtins);
	else
		execute_single_cmd(shell, cmd, builtins);
}
