/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/19 22:38:08 by ilkaddou         ###   ########.fr       */
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

static char	*get_var_value(char *var_name, t_env *env, int exit_status)
{
	t_env	*var;
	char	*var_value;

	if (ft_strcmp(var_name, "?") == 0)
		var_value = ft_itoa(exit_status);
	else
	{
		var = find_env_var(env, var_name);
		var_value = var ? ft_strdup(var->value) : ft_strdup("");
	}
	return (var_value);
}

static char	*process_dollar_in_quotes(char **ptr, t_env *env, int exit_status,
					char **start, char *content)
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
		var_value = get_var_value(var_name, env, exit_status);
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

char	*get_quoted_string(char **ptr, t_env *env, int exit_status)
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
			content = process_dollar_in_quotes(ptr, env, exit_status,
					&start, content);
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