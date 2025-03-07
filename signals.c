/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:18:53 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 15:30:44 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_sigint(int sig) // Gestion du CTRL + C
{
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
	    rl_redisplay();
    }
}

void	handle_eof(t_shell *shell) // Gestion du CTRL + D
{
	free_shell(shell);
	rl_clear_history();
	exit(0);
}

void setup_signals() // Gestion des signaux
{
    signal(SIGINT, handle_sigint); // Gestion du CTRL + C
    signal(SIGQUIT, SIG_IGN); // Gestion du CTRL + '\'
}
