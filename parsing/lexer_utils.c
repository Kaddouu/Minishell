/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:15 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/14 10:00:10 by ilkaddou         ###   ########.fr       */
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

// Fonction améliorée pour gérer les guillemets dans le contexte de export
int	handle_quotes(t_token **tokens, t_token **last, char **ptr)
{
	char quote;
	char *start;
	char *content;
	t_token *new_token;
	t_token *prev_token;
	int is_after_equal;

	if (!tokens || !last || !ptr || !*ptr)
		return (0);

	// Détermine si nous sommes après un signe égal en inspectant les tokens précédents
	is_after_equal = 0;
	prev_token = *tokens;
	while (prev_token && prev_token != *last)
		prev_token = prev_token->next;

	// Si le token précédent existe et contient un signe égal (comme dans export VAR="...")
	if (prev_token && prev_token->content && ft_strchr(prev_token->content,
			'='))
		is_after_equal = 1;

	quote = **ptr;
	(*ptr)++;
	start = *ptr;
	while (**ptr && **ptr != quote)
	{
        if (quote == '"' && **ptr == '$')
		{
            // Si nous sommes dans un contexte d'export (après un =),
			// conserver le $ tel quel plutôt que de le traiter comme une variable
            printf("kejk\n");
			if (is_after_equal)
            (*ptr)++;
			else
			{
				handle_env_var(tokens, last, ptr);
				start = *ptr;
			}
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

	// Si on est après un signe égal et que le token précédent contient '='
	if (is_after_equal && prev_token)
	{
		// On fusionne avec le token précédent (cas export VAR="...")
		char *new_content = ft_strjoin(prev_token->content, content);
		free(content);
		if (!new_content)
			return (0);

		free(prev_token->content);
		prev_token->content = new_content;
	}
	else
	{
		new_token = create_token(content, WORD);
		if (!new_token)
		{
			free(content);
			return (0);
		}
		add_token(tokens, last, new_token);
	}

	(*ptr)++;
	return (1);
}