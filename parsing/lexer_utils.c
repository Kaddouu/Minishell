/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:15 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 14:37:09 by ilkaddou         ###   ########.fr       */
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

void handle_env_var(t_token **tokens, t_token **last, char **ptr)
{
    char *start;
    char *var_name;

    (*ptr)++; // Move past $
    if (**ptr == '?' || ft_isalnum(**ptr) || **ptr == '_') // Valid variable name follows
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
    else // Lone $, treat as literal
    {
        add_token(tokens, last, create_token(ft_strdup("$"), WORD));
    }
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



// Fonction améliorée pour gérer les guillemets dans le contexte de export
// int	handle_quotes(t_token **tokens, t_token **last, char **ptr)
// {
// 	char	quote;
// 	char	*start;
// 	char	*content;
// 	t_token	*new_token;
// 	t_token	*prev_token;
// 	int		is_after_equal;
// 	char	*new_content;

// 	if (!tokens || !last || !ptr || !*ptr)
// 		return (0);
// 	is_after_equal = 0;
// 	prev_token = *tokens;
// 	while (prev_token && prev_token != *last)
// 		prev_token = prev_token->next;
// 	if (prev_token && prev_token->content && ft_strchr(prev_token->content,
// 			'='))
// 		is_after_equal = 1;
// 	quote = **ptr;
// 	(*ptr)++;
// 	start = *ptr;
// 	while (**ptr && **ptr != quote)
// 	{
// 		if (quote == '"' && **ptr == '$')
// 		{
// 			if (is_after_equal)
// 				(*ptr)++;
// 			else
// 			{
// 				handle_env_var(tokens, last, ptr);
// 				start = *ptr;
// 			}
// 		}
// 		else
// 			(*ptr)++;
// 	}
// 	if (!**ptr)
// 	{
// 		printf("Erreur : guillemet non fermé\n");
// 		return (0);
// 	}
// 	content = ft_substr(start, 0, *ptr - start);
// 	if (!content)
// 		return (0);
// 	if (is_after_equal && prev_token)
// 	{
// 		new_content = ft_strjoin(prev_token->content, content);
// 		free(content);
// 		if (!new_content)
// 			return (0);
// 		free(prev_token->content);
// 		prev_token->content = new_content;
// 	}
// 	else
// 	{
// 		new_token = create_token(content, WORD);
// 		if (!new_token)
// 			return (free(content), 0);
// 		add_token(tokens, last, new_token);
// 	}
// 	(*ptr)++;
// 	return (1);
// }

// In lexer_utils.c
char *get_quoted_string(char **ptr, t_env *env, int exit_status)
{
    char quote = **ptr; // Récupérer le type de guillemet (simple ou double)
    (*ptr)++;          // Passer le guillemet ouvrant
    char *start = *ptr;
    char *content = ft_strdup(""); // Chaîne vide pour accumuler le résultat

    while (**ptr && **ptr != quote)
    {
        if (quote == '"' && **ptr == '$') // Si guillemets doubles et $ trouvé
        {
            // Ajouter la partie avant le $ (s'il y en a une)
            if (*ptr > start)
            {
                char *temp = ft_substr(start, 0, *ptr - start);
                char *new_content = ft_strjoin(content, temp);
                free(content);
                free(temp);
                content = new_content;
            }
            (*ptr)++; // Passer le $
            // Vérifier si $ est suivi d'un nom de variable valide
            if (**ptr == '?' || ft_isalnum(**ptr) || **ptr == '_')
            {
                char *var_name;
                if (**ptr == '?') // Cas spécial pour $?
                {
                    var_name = ft_strdup("?");
                    (*ptr)++;
                }
                else // Cas général pour $HOME, $USER, etc.
                {
                    char *var_start = *ptr;
                    while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
                        (*ptr)++;
                    var_name = ft_substr(var_start, 0, *ptr - var_start);
                }
                // Récupérer la valeur de la variable
                char *var_value;
                if (ft_strcmp(var_name, "?") == 0)
                    var_value = ft_itoa(exit_status);
                else
                {
                    t_env *var = find_env_var(env, var_name);
                    var_value = var ? ft_strdup(var->value) : ft_strdup("");
                }
                free(var_name);
                char *new_content = ft_strjoin(content, var_value);
                free(content);
                free(var_value);
                content = new_content;
            }
            else // Lone $ inside double quotes
            {
                char *new_content = ft_strjoin(content, "$");
                free(content);
                content = new_content;
            }
            start = *ptr; // Mettre à jour le début pour la suite
        }
        else
        {
            (*ptr)++; // Avancer si pas de $
        }
    }
    // Ajouter la dernière partie avant le guillemet fermant
    if (*ptr > start)
    {
        char *temp = ft_substr(start, 0, *ptr - start);
        char *new_content = ft_strjoin(content, temp);
        free(content);
        free(temp);
        content = new_content;
    }
    if (**ptr != quote)
    {
        printf("Erreur : guillemet non fermé\n");
        free(content);
        return (NULL);
    }
    (*ptr)++; // Passer le guillemet fermant
    return (content);
}

void handle_argument(t_token **tokens, t_token **last, char **ptr, t_env *env, int exit_status)
{
    char *argument = ft_strdup(""); // Accumuler l'argument
    while (**ptr && !ft_isspace(**ptr) && **ptr != '|' && **ptr != '<' && **ptr != '>')
    {
        if (**ptr == '\'' || **ptr == '"') // Gestion des guillemets
        {
            char *quoted = get_quoted_string(ptr, env, exit_status);
            if (quoted)
            {
                char *temp = ft_strjoin(argument, quoted);
                free(argument);
                free(quoted);
                argument = temp;
            }
            else
            {
                free(argument);
                return;
            }
        }
        else if (**ptr == '$' && (*(*ptr + 1) == '?' || ft_isalnum(*(*ptr + 1)) || *(*ptr + 1) == '_')) // Variable d'environnement valide
        {
            (*ptr)++; // Passer le $
            char *var_name;
            if (**ptr == '?') // Cas spécial pour $?
            {
                var_name = ft_strdup("?");
                (*ptr)++;
            }
            else // Cas général pour $HOME, $USER, etc.
            {
                char *var_start = *ptr;
                while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
                    (*ptr)++;
                var_name = ft_substr(var_start, 0, *ptr - var_start);
            }
            // Récupérer la valeur de la variable
            char *var_value;
            if (ft_strcmp(var_name, "?") == 0)
                var_value = ft_itoa(exit_status);
            else
            {
                t_env *var = find_env_var(env, var_name);
                var_value = var ? ft_strdup(var->value) : ft_strdup("");
            }
            free(var_name);
            // Ajouter la valeur expansée à l'argument
            char *temp = ft_strjoin(argument, var_value);
            free(argument);
            free(var_value);
            argument = temp;
        }
        else // Caractères normaux ou $ seul
        {
            char *start = *ptr;
            while (**ptr && !ft_isspace(**ptr) && **ptr != '|' && **ptr != '<' && **ptr != '>' && 
                   !(**ptr == '$' && (*(*ptr + 1) == '?' || ft_isalnum(*(*ptr + 1)) || *(*ptr + 1) == '_')) &&
                   **ptr != '\'' && **ptr != '"')
                (*ptr)++;
            char *word = ft_substr(start, 0, *ptr - start);
            char *temp = ft_strjoin(argument, word);
            free(argument);
            free(word);
            argument = temp;
        }
    }
    if (argument && *argument)
        add_token(tokens, last, create_token(argument, WORD));
    else
        free(argument);
}
