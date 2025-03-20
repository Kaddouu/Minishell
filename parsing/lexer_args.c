/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/20 14:13:22 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_var_name(t_lexer_context *lexer)
{
	char	*var_name;
	char	*var_start;

	if (**lexer->ptr == '?')
	{
		var_name = ft_strdup("?");
		(*lexer->ptr)++;
	}
	else
	{
		var_start = *lexer->ptr;
		while (**lexer->ptr && (ft_isalnum(**lexer->ptr)
				|| **lexer->ptr == '_'))
			(*lexer->ptr)++;
		var_name = ft_substr(var_start, 0, *lexer->ptr - var_start);
	}
	return (var_name);
}

static char	*get_variable_value(t_lexer_context *lexer, char *var_name)
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

static char	*handle_dollar_var(t_lexer_context *lexer, char *argument)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

	(*lexer->ptr)++;
	if (**lexer->ptr == '?' || ft_isalnum(**lexer->ptr) || **lexer->ptr == '_')
	{
		var_name = extract_var_name(lexer);
		var_value = get_variable_value(lexer, var_name);
		free(var_name);
		temp = ft_strjoin(argument, var_value);
		return (free(argument), free(var_value), temp);
	}
	return (argument);
}

static char	*handle_normal_text(t_lexer_context *lexer, char *argument)
{
	char	*start;
	char	*word;
	char	*temp;

	start = *lexer->ptr;
	while (**lexer->ptr && !ft_isspace(**lexer->ptr) && **lexer->ptr != '|'
		&& **lexer->ptr != '<' && **lexer->ptr != '>' && !(**lexer->ptr == '$'
			&& ((*(*lexer->ptr + 1) == '?' || ft_isalnum(*(*lexer->ptr + 1))
					|| *(*lexer->ptr + 1) == '_'))) && **lexer->ptr != '\''
		&& **lexer->ptr != '"')
		(*lexer->ptr)++;
	word = ft_substr(start, 0, *lexer->ptr - start);
	temp = ft_strjoin(argument, word);
	free(argument);
	free(word);
	return (temp);
}

void	handle_argument(t_lexer_context *lexer)
{
	char	*argument;

	argument = ft_strdup("");
	while (**lexer->ptr && !ft_isspace(**lexer->ptr) && **lexer->ptr != '|'
		&& **lexer->ptr != '<' && **lexer->ptr != '>')
	{
		if (**lexer->ptr == '\'' || **lexer->ptr == '"')
			argument = handle_quoted_part(lexer, argument);
		else if (**lexer->ptr == '$' && (*(*lexer->ptr + 1) == '?'
				|| ft_isalnum(*(*lexer->ptr + 1)) || *(*lexer->ptr + 1) == '_'))
			argument = handle_dollar_var(lexer, argument);
		else
			argument = handle_normal_text(lexer, argument);
		if (!argument)
			return ;
	}
	if (argument && *argument)
		add_token(lexer->tokens, lexer->last, create_token(argument, WORD));
	else
		free(argument);
}
