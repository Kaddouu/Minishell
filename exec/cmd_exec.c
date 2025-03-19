/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:13:10 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/19 15:30:39 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			else if (ft_strcmp(cmd->args[0], "unset") == 0)
				return (ft_unset(cmd->args, shell));
			else if (ft_strcmp(cmd->args[0], "env") == 0)
				return (builtins[i].func(env_to_array(shell->env), shell));
			else
				return (builtins[i].func(cmd->args, shell));
		}
		i++;
	}
	return (-1);
}

void	execute_child_process(t_shell *shell, t_command *cmd, t_env *env,
		int heredoc_fd)
{
	char	*path;

	handle_redirection(cmd, heredoc_fd, shell);
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
	ft_putstr_fd("Execve failed: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd("\n", 2);
	free(path);
	exit(EXIT_FAILURE);
}

void	execute_external(t_shell *shell, t_command *cmd, t_env *env,
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
		execute_child_process(shell, cmd, env, heredoc_fd);
	else
	{
		if (heredoc_fd != -1)
			close(heredoc_fd);
		waitpid(pid, &shell->exit_status, 0);
		if (WIFEXITED(shell->exit_status))
			shell->exit_status = WEXITSTATUS(shell->exit_status);
		g_shell_state = 0;
	}
}

void	execute_single_command(t_shell *shell, t_command *cmd,
		t_builtin *builtins)
{
	int	heredoc_fd;
	int	saved_stdin;
	int	saved_stdout;

	heredoc_fd = -1;
	if (cmd->heredoc)
		heredoc_fd = handle_heredoc(shell, cmd);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
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

void	execute_commands(t_shell *shell, t_builtin *builtins)
{
	t_command	*cmd;

	cmd = shell->cmds;
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (cmd->next)
		execute_pipeline(shell, builtins);
	else
		execute_single_command(shell, cmd, builtins);
}
