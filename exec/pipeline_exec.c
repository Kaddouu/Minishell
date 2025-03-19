/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:12:45 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/19 15:31:42 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_in_child(t_shell *shell, t_command *cmd, t_builtin *builtins,
		t_exec_data *exec_data)
{
	char	*path;
	int		status;

	setup_pipeline_fds(exec_data->pipeline->prev_fd,
		exec_data->pipeline->pipe_fd[0], exec_data->pipeline->pipe_fd, cmd);
	handle_redirection(cmd, -1, shell);
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

void	handle_pipeline_command(t_shell *shell, t_command *cmd,
		t_builtin *builtins, t_exec_data *exec_data)
{
	int	heredoc_fd;

	heredoc_fd = -1;
	if (cmd->heredoc)
		heredoc_fd = handle_heredoc(shell, cmd);
	if (cmd->next)
		setup_pipe(exec_data->pipeline->pipe_fd, shell);
	exec_data->pipeline->pids[exec_data->index] = fork();
	if (exec_data->pipeline->pids[exec_data->index] == -1)
	{
		ft_putstr_fd("Fork failed\n", 2);
		shell->exit_status = 1;
		return ;
	}
	if (exec_data->pipeline->pids[exec_data->index] == 0)
		execute_in_child(shell, cmd, builtins, exec_data);
	if (heredoc_fd != -1)
		close(heredoc_fd);
	if (exec_data->pipeline->prev_fd != STDIN_FILENO)
		close(exec_data->pipeline->prev_fd);
	manage_pipe_fds(&exec_data->pipeline->prev_fd, exec_data->pipeline->pipe_fd,
		cmd);
}

void	execute_pipeline(t_shell *shell, t_builtin *builtins)
{
	t_exec_data		exec_data;
	t_pipeline_data	pipeline;
	t_command		*cmd;
	int				cmd_count;

	cmd_count = count_commands(shell->cmds);
	pipeline.pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipeline.pids)
	{
		ft_putstr_fd("Memory allocation failed\n", 2);
		return ;
	}
	pipeline.prev_fd = STDIN_FILENO;
	g_shell_state = 1;
	exec_data.shell = shell;
	exec_data.builtins = builtins;
	exec_data.pipeline = &pipeline;
	exec_data.index = 0;
	cmd = shell->cmds;
	while (cmd)
	{
		handle_pipeline_command(shell, cmd, builtins, &exec_data);
		cmd = cmd->next;
		exec_data.index++;
	}
	wait_for_children(pipeline.pids, cmd_count, shell);
	free(pipeline.pids);
}
