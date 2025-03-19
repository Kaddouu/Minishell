/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:05 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/19 13:48:47 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (path)
	{
		ft_putstr_fd(path, 2);
	}
	ft_putendl_fd(": No such file or directory", 2);
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;

	(void)shell;
	if (!args[1] || strcmp(args[1], "~") == 0)
		path = getenv("HOME");
	else
		path = args[1];
	if (!path || chdir(path) != 0)
	{
		print_cd_error(path);
		return (1);
	}
	return (0);
}
