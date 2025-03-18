/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:15 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 14:56:14 by ilkaddou         ###   ########.fr       */
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

char	*get_quoted_string(char **ptr, t_env *env, int exit_status)
{
	char	quote;
	char	*start;
	char	*content;
	char	*temp;
	char	*new_content;
	char	*var_name;
	char	*var_start;
	char	*var_value;
	t_env	*var;

	quote = **ptr;
	(*ptr)++;
	start = *ptr;
	content = ft_strdup("");
	while (**ptr && **ptr != quote)
	{
		if (quote == '"' && **ptr == '$')
		{
			if (*ptr > start)
			{
				temp = ft_substr(start, 0, *ptr - start);
				new_content = ft_strjoin(content, temp);
				free(content);
				free(temp);
				content = new_content;
			}
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
					var_start = *ptr;
					while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
						(*ptr)++;
					var_name = ft_substr(var_start, 0, *ptr - var_start);
				}
				if (ft_strcmp(var_name, "?") == 0)
					var_value = ft_itoa(exit_status);
				else
				{
					var = find_env_var(env, var_name);
					var_value = var ? ft_strdup(var->value) : ft_strdup("");
				}
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
			start = *ptr;
		}
		else
		{
			(*ptr)++;
		}
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

void	handle_argument(t_token **tokens, t_token **last, char **ptr,
		t_env *env, int exit_status)
{
	char	*quoted;
	char	*temp;
	char	*var_name;
	char	*var_start;
	char	*var_value;
	t_env	*var;
	char	*start;
	char	*word;
	char	*argument;

	argument = ft_strdup("");
	while (**ptr && !ft_isspace(**ptr) && **ptr != '|' && **ptr != '<'
		&& **ptr != '>')
	{
		if (**ptr == '\'' || **ptr == '"')
		{
			quoted = get_quoted_string(ptr, env, exit_status);
			if (quoted)
			{
				temp = ft_strjoin(argument, quoted);
				free(argument);
				free(quoted);
				argument = temp;
			}
			else
			{
				free(argument);
				return ;
			}
		}
		else if (**ptr == '$' && (*(*ptr + 1) == '?' || ft_isalnum(*(*ptr + 1))
				|| *(*ptr + 1) == '_'))
		{
			(*ptr)++;
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
			if (ft_strcmp(var_name, "?") == 0)
				var_value = ft_itoa(exit_status);
			else
			{
				var = find_env_var(env, var_name);
				var_value = var ? ft_strdup(var->value) : ft_strdup("");
			}
			free(var_name);
			temp = ft_strjoin(argument, var_value);
			free(argument);
			free(var_value);
			argument = temp;
		}
		else
		{
			start = *ptr;
			while (**ptr && !ft_isspace(**ptr) && **ptr != '|' && **ptr != '<'
				&& **ptr != '>' && !(**ptr == '$' && (*(*ptr + 1) == '?'
						|| ft_isalnum(*(*ptr + 1)) || *(*ptr + 1) == '_'))
				&& **ptr != '\'' && **ptr != '"')
				(*ptr)++;
			word = ft_substr(start, 0, *ptr - start);
			temp = ft_strjoin(argument, word);
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
