/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:37:30 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/20 13:37:46 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t');
}

void	handle_exit_status(t_token *token, int exit_status, t_token **next)
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

void	handle_env_expansion(t_token *token, t_env *env, t_token **next)
{
	t_env	*var;
	t_token	*next_token;

	var = find_env_var(env, token->content);
	free(token->content);
	if (var)
		token->content = ft_strdup(var->value);
	else
		token->content = ft_strdup("");
	token->type = WORD;
	next_token = token->next;
	*next = next_token;
}

void	merge_env_tokens(t_token *current, t_token *next, t_env *env,
		int exit_status)
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
		if (var)
			next->content = ft_strdup(var->value);
		else
			next->content = ft_strdup("");
	}
	next->type = WORD;
	temp = ft_strjoin(current->content, next->content);
	free(current->content);
	current->content = temp;
	current->next = next->next;
	free(next->content);
	free(next);
}
