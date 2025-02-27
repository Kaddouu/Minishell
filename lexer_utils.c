/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:15 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/27 14:46:26 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	(*ptr)++; // Avancer après $
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
	// Avancer jusqu'à un délimiteur
	while (**ptr && !ft_isspace(**ptr) && **ptr != '|' && **ptr != '<'
		&& **ptr != '>' && **ptr != '\'' && **ptr != '"' && **ptr != '$')
		(*ptr)++;
	// Extraire le mot
	word = ft_substr(start, 0, *ptr - start);
	add_token(tokens, last, create_token(word, WORD));
}

void	handle_quotes(t_token **tokens, t_token **last, char **ptr)
{
	char	quote;
	char	*start;
	char	*content;

	quote = **ptr;
	(*ptr)++; // Avancer après le guillemet
	start = *ptr;
	// Trouver la fin de la chaîne entre guillemets
	while (**ptr && **ptr != quote)
	{
		// Gérer les variables d'environnement uniquement pour les guillemets doubles
		if (quote == '"' && **ptr == '$')
		{
			handle_env_var(tokens, last, ptr);
			start = *ptr; // Réinitialiser le début après l'expansion
		}
		(*ptr)++;
	}
	if (!**ptr) // Erreur: guillemet non fermé
		return ; // Gérer l'erreur
	// Extraire le contenu entre guillemets
	content = ft_substr(start, 0, *ptr - start);
	if (quote == '"') {
		// Si c'est un guillemet double, ajouter comme ENV
		add_token(tokens, last, create_token(content, WORD)); // Vous pouvez changer WORD en ENV si nécessaire
	} else {
		// Si c'est un guillemet simple, ajouter comme WORD
		add_token(tokens, last, create_token(content, WORD));
	}
	(*ptr)++; // Avancer après le guillemet fermant
}
