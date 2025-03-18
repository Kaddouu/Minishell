/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:05 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 15:14:10 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	int i;

	(void)shell;
	i = 0;
	while (args[i])
	{
		i++;
	}
	if (!args[1])
		return (1);
	path = args[1];
	if (i > 2)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": too many arguments\n", 2);
		return (1);
	}
	if (chdir(path) != 0 || i > 2)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}
