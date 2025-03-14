/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:53 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/14 12:34:15 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_env_name(char *str)
{
	int		i;
	char	*equals;

	if (!str || !str[0])
		return (0);
	equals = ft_strchr(str, '=');
	if (equals)
	{
		i = 0;
		while (&str[i] < equals)
		{
			if (i == 0 && isdigit(str[i]))
				return (0);
			if (!isalnum(str[i]) && str[i] != '_')
				return (0);
			i++;
		}
	}
	else
	{
		if (isdigit(str[0]))
			return (0);
		i = 0;
		while (str[i])
		{
			if (!isalnum(str[i]) && str[i] != '_')
				return (0);
			i++;
		}
	}
	return (1);
}

static void	remove_env_var(t_env **env, char *var_name)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	if (!args[1])
		return (0);
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			ret = 1;
		}
		else
			remove_env_var(&shell->env, args[i]);
		i++;
	}
	return (ret);
}
