/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:57 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 13:38:20 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_flag_n(char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	ft_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	while (args[i] && is_flag_n(args[i]))
	{
		newline = 0;
		i++;
	}
	if (args[i] && ft_strcmp(args[i], "$?") == 0)
	{
		ft_putstr_fd(ft_itoa(shell->exit_status), 1);
		if (args[++i])
			ft_putstr_fd(" ", 1);
		else
		{
			if (newline)
				ft_putstr_fd("\n", 1);
			return (0);
		}
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1] && args[i][0] != '\'')
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
