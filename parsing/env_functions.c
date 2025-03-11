/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:29:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/11 11:44:19 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*find_env_var(t_env *env, char *var_name)
{
	while (env)
	{
		if (ft_strcmp(env->key, var_name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	expand_all_env_vars(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded;
	t_env	*var;

	current = tokens;
	while (current)
	{
		if (current->type == ENV)
		{
			var = find_env_var(env, current->content);
			if (var)
			{
				free(current->content);
				current->content = ft_strdup(var->value);
				current->type = WORD;
			}
			else
			{
				free(current->content);
				current->content = ft_strdup("");
				current->type = WORD;
			}
		}
		else if (current->type == WORD)
		{
			if (ft_strchr(current->content, '$'))
			{
				expanded = expand_variables(current->content, env);
				free(current->content);
				current->content = expanded;
			}
		}
		current = current->next;
	}
}
