/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/05 14:29:21 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_env_name(char *str)
{
	int	i;

	if (!str || !(*str) || ft_isdigit(str[0]))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static char	**extend_env(char **old_env, char *new_var)
{
	char	**new_env;
	int		len;
	int		i;

	len = 0;
	while (old_env[len])
		len++;
	new_env = malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_env[i] = old_env[i];
		i++;
	}
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free(old_env);
	return (new_env);
}

int	ft_export(char **args, char ***envp)
{
	int	i;

	if (!args[1])
	{
		i = 0;
		while ((*envp)[i++])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd((*envp)[i], 1);
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (1);
		}
		*envp = extend_env(*envp, args[i++]);
		if (!*envp)
		{
			ft_putendl_fd("minishell: export: memory allocation error", 2);
			return (1);
		}
	}
	return (0);
}
