/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 22:39:41 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/20 12:14:12 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input(t_shell *shell, char *line)
{
	if (!line || !shell)
		return (-1);
	if (ft_strcmp(line, "exit") == 0)
		return (-1);
	shell->tokens = lexer(line, shell->env, shell->exit_status);
	if (!shell->tokens)
		return (0);
	expand_all_env_vars(shell->tokens, shell->env, shell->exit_status);
	shell->cmds = parser(shell->tokens);
	if (!shell->cmds)
	{
		free_token(shell->tokens);
		shell->tokens = NULL;
		return (0);
	}
	free_token(shell->tokens);
	shell->tokens = NULL;
	free_command(shell->cmds);
	shell->cmds = NULL;
	return (0);
}
