/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:37:58 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/26 17:37:36 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handle_sigint;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_eof(t_shell *shell)
{
	free_shell(shell);
	rl_clear_history();
	exit(0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell	*shell;
// 	char	*line;

// 	(void)argc;
// 	(void)argv;
// 	shell = init_shell(envp);
// 	if (!shell)
// 		return (1);
// 	setup_signals();
// 	while (shell->running)
// 	{
// 		line = readline("minishell> ");
// 		if (!line)
// 			handle_eof(shell);
// 		if (ft_strlen(line) > 0)
// 		{
// 			add_history(line);
// 			if (handle_input(shell, line) == -1)
// 				shell->running = 0;
// 		}
// 		free(line);
// 	}
// 	free_shell(shell);
// 	rl_clear_history();
// 	return (shell->exit_status);
// }
