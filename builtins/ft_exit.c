/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:50:16 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 12:01:05 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_isdigitstr(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}


int	ft_exit(char **args, t_shell *shell)
{
	int	status;

	(void)shell;
	status = 0;
	
	if (args[2])
	{
		ft_putstr_fd(" too many arguments", 1);
		exit (1);
	}
	if (args[1])
	{
		if (!ft_isdigitstr(args[1]))
		{
			ft_putendl_fd(" minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit(2);
		}
		if (ft_atoi(args[1]) > 255)
		status = ft_atoi(args[1]) % 256;
	}
	ft_putendl_fd("exit", 2);
	exit(status);
}
