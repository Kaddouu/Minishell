/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:37:58 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/20 12:13:56 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **ev)
{
	t_shell		*shell;
	t_builtin	*builtins;
	char		*line;

	(void)ac;
	(void)av;
	setup_signals();
	if (!ev || !ev[0])
		shell = init_shell(NULL);
	else
		shell = init_shell(ev);
	builtins = init_builtins();
	if (!shell)
		return (1);
	while (shell->running)
	{
		line = readline("minishell> ");
		if (line == NULL)
			handle_eof(shell);
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			shell->tokens = lexer(line, shell->env, shell->exit_status);
			if (shell->tokens)
			{
				expand_all_env_vars(shell->tokens, shell->env,
					shell->exit_status);
				shell->cmds = parser(shell->tokens);
				if (shell->cmds)
					execute_commands(shell, builtins);
				free_token(shell->tokens);
				free_command(shell->cmds);
				shell->tokens = NULL;
				shell->cmds = NULL;
			}
		}
		free(line);
	}
	free_builtins(builtins);
	free_shell(shell);
	rl_clear_history();
	return (0);
}
