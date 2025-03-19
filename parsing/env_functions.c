/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:29:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/19 22:47:37 by ilkaddou         ###   ########.fr       */
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

static void	handle_exit_status(t_token *token, int exit_status, t_token **next)
{
	t_token	*next_token;
	char	*temp;

	free(token->content);
	token->content = ft_itoa(exit_status);
	token->type = WORD;
	next_token = token->next;
	if (next_token && next_token->type == WORD
		&& !is_special_char(next_token->content[0]))
	{
		temp = ft_strjoin(token->content, next_token->content);
		free(token->content);
		token->content = temp;
		token->next = next_token->next;
		free(next_token->content);
		free(next_token);
	}
	*next = token->next;
}

static void	handle_env_expansion(t_token *token, t_env *env, t_token **next)
{
	t_env	*var;
	t_token	*next_token;

	var = find_env_var(env, token->content);
	free(token->content);
	token->content = var ? ft_strdup(var->value) : ft_strdup("");
	token->type = WORD;
	next_token = token->next;
	*next = next_token;
}

static void	merge_env_tokens(t_token *current, t_token *next,
				t_env *env, int exit_status)
{
	t_env	*var;
	char	*temp;

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
