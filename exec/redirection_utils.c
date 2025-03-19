/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:25:33 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/19 15:30:03 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_input_redirection(t_command *cmd, int heredoc_fd)
{
	int	fd;

	if (cmd->input && heredoc_fd == -1)
	{
		fd = open(cmd->input, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->input, 2);
			ft_putendl_fd(": No such file or directory", 2);
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (heredoc_fd != -1)
	{
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
}

static void	handle_output_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->output)
	{
		fd = open(cmd->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("Redirection failed: Cannot open ", 2);
			ft_putstr_fd(cmd->output, 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

static void	handle_append_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->append)
	{
		fd = open(cmd->append, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("Redirection failed: Cannot open ", 2);
			ft_putstr_fd(cmd->append, 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	handle_redirection(t_command *cmd, int heredoc_fd, t_shell *shell)
{
	(void)shell;
	handle_input_redirection(cmd, heredoc_fd);
	handle_output_redirection(cmd);
	handle_append_redirection(cmd);
}
