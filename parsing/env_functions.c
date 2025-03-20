/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:29:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/20 13:37:22 by ilkaddou         ###   ########.fr       */
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

void	expand_all_env_vars(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		if (current->type == ENV)
		{
			if (ft_strcmp(current->content, "?") == 0)
				handle_exit_status(current, exit_status, &next);
			else
				handle_env_expansion(current, env, &next);
			while (next && next->type == ENV)
			{
				merge_env_tokens(current, next, env, exit_status);
				next = current->next;
			}
		}
		current = current->next;
	}
}
