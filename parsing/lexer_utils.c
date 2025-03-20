/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:15 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/20 14:09:27 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_token(char *content, t_type type)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = content;
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **tokens, t_token **last, t_token *new_token)
{
	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		*last = new_token;
	}
	else
	{
		(*last)->next = new_token;
		*last = new_token;
	}
}

void	handle_env_var(t_token **tokens, t_token **last, char **ptr)
{
	char	*start;
	char	*var_name;

	(*ptr)++;
	if (**ptr == '?' || ft_isalnum(**ptr) || **ptr == '_')
	{
		if (**ptr == '?')
		{
			var_name = ft_strdup("?");
			(*ptr)++;
		}
		else
		{
			start = *ptr;
			while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
				(*ptr)++;
			var_name = ft_substr(start, 0, *ptr - start);
		}
		add_token(tokens, last, create_token(var_name, ENV));
	}
	else
		add_token(tokens, last, create_token(ft_strdup("$"), WORD));
}

char	*handle_quoted_part(t_lexer_context *lexer, char *argument)
{
	char	*quoted;
	char	*temp;

	quoted = get_quoted_string(lexer->ptr, lexer->env);
	if (!quoted)
	{
		free(argument);
		return (NULL);
	}
	temp = ft_strjoin(argument, quoted);
	free(argument);
	free(quoted);
	return (temp);
}

void	handle_word(t_token **tokens, t_token **last, char **ptr)
{
	char	*start;
	char	*word;

	start = *ptr;
	while (**ptr && !ft_isspace(**ptr) && **ptr != '|' && **ptr != '<'
		&& **ptr != '>' && **ptr != '\'' && **ptr != '"' && **ptr != '$')
		(*ptr)++;
	word = ft_substr(start, 0, *ptr - start);
	if (!word)
		return ;
	add_token(tokens, last, create_token(word, WORD));
}
