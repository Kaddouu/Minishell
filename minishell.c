/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:37:58 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 17:58:15 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	main(int ac, char **av, char **ev)
// {
// 	t_shell	*shell;
// 	t_builtin	*builtins;
// 	char	*line;

// 	(void)ac;
// 	(void)av;
// 	setup_signals();
// 	shell = init_shell(ev);
// 	builtins = init_builtins();
// 	if (!shell)
// 		return (1);
// 	while (shell->running)
// 	{
// 		line = readline("minishell> ");
// 		if (line == NULL)
// 			handle_eof(shell);
// 		if (ft_strlen(line) > 0)
// 		{
// 			add_history(line);
// 			shell->tokens = lexer(line);
// 			shell->cmds = parser(shell->tokens);
// 			execute_commands(shell, builtins);
// 		}
// 		free(line);
// 	}
// 	free_shell(shell);
// 	rl_clear_history();
// 	return (shell->exit_status);
// }

int	main(int ac, char **av, char **ev)
{
	t_shell		*shell;
	t_builtin	*builtins;
	char		*line;

	(void)ac;
	(void)av;
	setup_signals(); // gestion des signaux
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
			handle_eof(shell); // on gere le CTRL + D, c'est un signal qui doit pouvoir intervenir tout le temps
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			shell->tokens = lexer(line);
			shell->cmds = parser(shell->tokens);
			if (shell->cmds) // on lexe puis on parse et on verifie que y'a bien quelque chose
				execute_commands(shell, builtins);
			free_token(shell->tokens);
			free_command(shell->cmds);
			shell->tokens = NULL;
			shell->cmds = NULL;
		}
		free(line);
	}
	free_builtins(builtins);
	free_shell(shell);
	rl_clear_history();
	return (shell->exit_status);
}
