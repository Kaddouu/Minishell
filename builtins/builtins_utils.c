/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:17:06 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/14 15:30:47 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_name_with_equals(char *str, int i)
{
	while (str[++i] != '=')
	{
		if (i == 0 && ft_isdigit(str[i]))
			return (0);
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
	}
	return (1);
}

int	export_error(char *key)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(key, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

void	handle_quoted_value(char **value)
{
	char	*temp;

	if ((*value)[0] == '"' && (*value)[ft_strlen(*value) - 1] == '"')
	{
		temp = ft_substr(*value, 1, ft_strlen(*value) - 2);
		free(*value);
		*value = temp;
	}
}

t_env	*find_var(t_env *env, char *key)
{
	while (env && ft_strcmp(env->key, key) != 0)
		env = env->next;
	return (env);
}

void	remove_env_var(t_env **env, char *var_name)
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
