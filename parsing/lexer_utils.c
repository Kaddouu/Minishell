/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:15 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/11 11:44:57 by ilkaddou         ###   ########.fr       */
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
	start = *ptr;
	while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
		(*ptr)++;
	var_name = ft_substr(start, 0, *ptr - start);
	add_token(tokens, last, create_token(var_name, ENV));
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
	add_token(tokens, last, create_token(word, WORD));
}

int	handle_quotes(t_token **tokens, t_token **last, char **ptr)
{
	char	quote;
	char	*start;
	char	*content;
	t_token	*new_token;

	if (!tokens || !last || !ptr || !*ptr)
		return (0);
	quote = **ptr;
	(*ptr)++;
	start = *ptr;
	while (**ptr && **ptr != quote)
	{
		if (quote == '"' && **ptr == '$')
		{
			handle_env_var(tokens, last, ptr);
			start = *ptr;
		}
		else
			(*ptr)++;
	}
	if (!**ptr)
	{
		printf("Erreur : guillemet non fermé\n");
		return (0);
	}
	content = ft_substr(start, 0, *ptr - start);
	if (!content)
		return (0);
	new_token = create_token(content, WORD);
	if (!new_token)
	{
		free(content);
		return (0);
	}
	add_token(tokens, last, new_token);
	(*ptr)++;
	return (1);
}
