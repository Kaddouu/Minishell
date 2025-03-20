/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/20 15:59:28 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_var_name(char **ptr)
{
	char	*var_name;
	char	*var_start;

	if (**ptr == '?')
	{
		var_name = ft_strdup("?");
		(*ptr)++;
	}
	else
	{
		var_start = *ptr;
		while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
			(*ptr)++;
		var_name = ft_substr(var_start, 0, *ptr - var_start);
	}
	return (var_name);
}

static char	*get_var_value(char *var_name, t_lexer_context *lexer)
{
	char	*var_value;
	t_env	*var;

	if (ft_strcmp(var_name, "?") == 0)
		var_value = ft_itoa(lexer->exit_status);
	else
	{
		var = find_env_var(lexer->env, var_name);
		if (var)
			var_value = ft_strdup(var->value);
		else
			var_value = ft_strdup("");
	}
	return (var_value);
}

static char	*process_dollar_in_quotes(char **ptr, t_lexer_context *lexer, char **start,
		char *content)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	char	*new_content;

	if (*ptr > *start)
	{
		temp = ft_substr(*start, 0, *ptr - *start);
		new_content = ft_strjoin(content, temp);
		free(content);
		free(temp);
		content = new_content;
	}
	(*ptr)++;
	if (**ptr == '?' || ft_isalnum(**ptr) || **ptr == '_')
	{
		var_name = extract_var_name(ptr);
		var_value = get_var_value(var_name, lexer);
		free(var_name);
		new_content = ft_strjoin(content, var_value);
		free(content);
		free(var_value);
		content = new_content;
	}
	else
	{
		new_content = ft_strjoin(content, "$");
		free(content);
		content = new_content;
	}
	*start = *ptr;
	return (content);
}

char	*get_quoted_string(char **ptr, t_lexer_context *lexer)
{
	char	quote;
	char	*start;
	char	*content;
	char	*temp;
	char	*new_content;

	quote = **ptr;
	(*ptr)++;
	start = *ptr;
	content = ft_strdup("");
	while (**ptr && **ptr != quote)
	{
		if (quote == '"' && **ptr == '$')
			content = process_dollar_in_quotes(ptr, lexer, &start, content);
		else
			(*ptr)++;
	}
	if (*ptr > start)
	{
		temp = ft_substr(start, 0, *ptr - start);
		new_content = ft_strjoin(content, temp);
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
	(*ptr)++;
	return (content);
}
