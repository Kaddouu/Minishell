/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:29:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/19 10:43:42 by ilkaddou         ###   ########.fr       */
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
static int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t');
}

void	expand_all_env_vars(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	t_token	*next;
	char	*temp;
	t_env	*var;

	current = tokens;
	while (current)
	{
		if (current->type == ENV)
		{
			if (ft_strcmp(current->content, "?") == 0) // Special case for $?
			{
				free(current->content);
				current->content = ft_itoa(exit_status);
				current->type = WORD;
				next = current->next;
				if (next && next->type == WORD
					&& !is_special_char(next->content[0]))
				{
					temp = ft_strjoin(current->content, next->content);
					free(current->content);
					current->content = temp;
					current->next = next->next;
					free(next->content);
					free(next);
				}
			}
			else
			{
				var = find_env_var(env, current->content);
				free(current->content);
				current->content = var ? ft_strdup(var->value) : ft_strdup("");
			}
			current->type = WORD;
			next = current->next;
			while (next && next->type == ENV)
			{
				if (ft_strcmp(next->content, "?") == 0)
				{
					free(next->content);
					next->content = ft_itoa(exit_status);
				}
				else
				{
					var = find_env_var(env, next->content);
					free(next->content);
					next->content = var ? ft_strdup(var->value) : ft_strdup("");
				}
				next->type = WORD;
				temp = ft_strjoin(current->content, next->content);
				free(current->content);
				current->content = temp;
				current->next = next->next;
				free(next->content);
				free(next);
				next = current->next;
			}
		}
		current = current->next;
	}
}
