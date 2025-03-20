/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/20 11:52:36 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_token_delimiter(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

static char	*handle_quoted_part(char **ptr, t_env *env, int exit_status,
					char *argument)
{
	char	*quoted;
	char	*temp;

	quoted = get_quoted_string(ptr, env, exit_status);
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

static char	*handle_dollar_var(char **ptr, t_env *env,char *argument)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

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
			char *var_start = *ptr;
			while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
				(*ptr)++;
			var_name = ft_substr(var_start, 0, *ptr - var_start);
		}
		if (ft_strcmp(var_name, "?") == 0)
			var_value = ft_itoa(env->shell->exit_status);
		else
		{
			t_env *var = find_env_var(env, var_name);
			if (var)
				var_value = ft_strdup(var->value);
			else
				var_value = ft_strdup("");
		}
		free(var_name);
		temp = ft_strjoin(argument, var_value);
		return (free(argument), free(var_value), temp);
	}
	return (argument);
}

static char	*handle_normal_text(char **ptr, char *argument)
{
	char	*start;
	char	*word;
	char	*temp;

	start = *ptr;
	while (**ptr && !is_token_delimiter(**ptr) &&
		!(**ptr == '$' && (*(*ptr + 1) == '?' || ft_isalnum(*(*ptr + 1))
				|| *(*ptr + 1) == '_')) && **ptr != '\'' && **ptr != '"')
		(*ptr)++;
	word = ft_substr(start, 0, *ptr - start);
	temp = ft_strjoin(argument, word);
	free(argument);
	free(word);
	return (temp);
}

void	handle_argument(t_token **tokens, t_token **last, char **ptr,
		t_env *env, int exit_status)
{
	char	*argument;

	argument = ft_strdup("");
	while (**ptr && !is_token_delimiter(**ptr))
	{
		if (**ptr == '\'' || **ptr == '"')
			argument = handle_quoted_part(ptr, env, exit_status, argument);
		else if (**ptr == '$' && (*(*ptr + 1) == '?' || ft_isalnum(*(*ptr + 1))
				|| *(*ptr + 1) == '_'))
			argument = handle_dollar_var(ptr, env, argument);
		else
			argument = handle_normal_text(ptr, argument);
		
		if (!argument)
			return ;
	}
	if (argument && *argument)
		add_token(tokens, last, create_token(argument, WORD));
	else
		free(argument);
}
