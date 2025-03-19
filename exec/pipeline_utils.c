/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:09:18 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/19 15:29:54 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	setup_pipeline_fds(int prev_fd, int heredoc_fd, int *pipe_fd,
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

void	setup_pipe(int *pipe_fd, t_shell *shell)
{
	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		shell->exit_status = 1;
	}
}

void	manage_pipe_fds(int *prev_fd, int *pipe_fd, t_command *cmd)
{
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
}

void	wait_for_children(pid_t *pids, int cmd_count, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &shell->exit_status, 0);
		if (WIFEXITED(shell->exit_status))
			shell->exit_status = WEXITSTATUS(shell->exit_status);
		i++;
	}
	g_shell_state = 0;
}
