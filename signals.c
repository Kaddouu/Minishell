/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:18:53 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/15 15:49:19 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_shell_state = 0;

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		if (g_shell_state == 0)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else
			g_shell_state = 2;
	}
}

void	handle_eof(t_shell *shell)
{
	free_shell(shell);
	rl_clear_history();
	exit(0);
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
