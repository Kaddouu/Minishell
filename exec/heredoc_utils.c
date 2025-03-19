/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:25:59 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/19 15:31:39 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_heredoc_line(char *line, t_command *cmd, int pipe_fd,
		t_shell *shell)
{
	char	*expanded_line;

	if (cmd->expand_heredoc)
	{
		expanded_line = expand_variables(line, shell->env);
		if (expanded_line)
		{
			ft_putstr_fd(expanded_line, pipe_fd);
			ft_putchar_fd('\n', pipe_fd);
			free(expanded_line);
		}
	}
	else
	{
		ft_putstr_fd(line, pipe_fd);
		ft_putchar_fd('\n', pipe_fd);
	}
	free(line);
}

static int	read_heredoc_lines(t_command *cmd, int write_fd, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file\n",
				2);
			break ;
		}
		if (ft_strcmp(line, cmd->heredoc) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(line, cmd, write_fd, shell);
	}
	return (0);
}

int	handle_heredoc(t_shell *shell, t_command *cmd)
{
	int	pipe_fd[2];
	int	write_fd;
	int	ret;

	if (!cmd->heredoc)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("Pipe failed for heredoc\n", 2);
		shell->exit_status = 1;
		return (-1);
	}
	write_fd = pipe_fd[1];
	ret = read_heredoc_lines(cmd, write_fd, shell);
	close(write_fd);
	if (ret != 0)
		return (-1);
	return (pipe_fd[0]);
}
