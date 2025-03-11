/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:52:16 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/11 13:07:35 by ilkaddou         ###   ########.fr       */
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

void	execute_external(t_shell *shell, t_command *cmd, t_env *env)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("Fork failed\n", 2);
		shell->exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		handle_redirection(cmd);
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
		exit(1);
	}
	else
	{
		waitpid(pid, &shell->exit_status, 0);
		if (WIFEXITED(shell->exit_status))
			shell->exit_status = WEXITSTATUS(shell->exit_status);
	}
}

void	execute_pipeline(t_shell *shell, t_builtin *builtins)
{
	t_command	*cmd;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		*pids;
	int			cmd_count;
	int			i;
	t_command	*tmp;
	int			status;
	char		*path;

	cmd = shell->cmds;
	prev_fd = STDIN_FILENO;
	cmd_count = 0;
	tmp = cmd;
	while (tmp)
	{
		cmd_count++;
		tmp = tmp->next;
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		ft_putstr_fd("Memory allocation failed\n", 2);
		return ;
	}
	i = 0;
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
		{
			ft_putstr_fd("Pipe failed\n", 2);
			shell->exit_status = 1;
			break ;
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			ft_putstr_fd("Fork failed\n", 2);
			shell->exit_status = 1;
			break ;
		}
		if (pids[i] == 0)
		{
			if (prev_fd != STDIN_FILENO)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmd->next)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			handle_redirection(cmd);
			if (is_builtin(cmd->args[0], builtins))
			{
				status = execute_builtin(shell, cmd, builtins);
				exit(status);
			}
			else
			{
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
				ft_putstr_fd("Execve failed: ", 2);
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd("\n", 2);
				free(path);
				exit(1);
			}
		}
		if (prev_fd != STDIN_FILENO)
			close(prev_fd);
		if (cmd->next)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
		cmd = cmd->next;
		i++;
	}
	for (int j = 0; j < cmd_count; j++)
	{
		waitpid(pids[j], &shell->exit_status, 0);
		if (WIFEXITED(shell->exit_status))
			shell->exit_status = WEXITSTATUS(shell->exit_status);
	}
	free(pids);
}

void	execute_commands(t_shell *shell, t_builtin *builtins)
{
	t_command	*cmd;

	cmd = shell->cmds;
	if (!cmd)
		return ;
	if (cmd->next)
		execute_pipeline(shell, builtins);
	else
	{
		handle_heredoc(shell, cmd);
		if (is_builtin(cmd->args[0], builtins))
			shell->exit_status = execute_builtin(shell, cmd, builtins);
		else
			execute_external(shell, cmd, shell->env);
	}
}
