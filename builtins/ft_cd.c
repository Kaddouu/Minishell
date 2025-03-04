/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:05 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/04 15:44:27 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static char	*get_home_dir(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "HOME=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

int	ft_cd(char **args, char ***env)
{
	char	*path;
	char	*cwd;

	if (!args[1])
	{
		path = get_home_dir(*env);
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		print_chdir_error(path);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_putstr_fd("minishell: cd: cannot get current directory\n", 2);
		return (1);
	}
	return (0);
}
