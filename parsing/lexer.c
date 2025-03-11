/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:30:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/11 11:45:12 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// t_token	*lexer(char *input)
// {
// 	t_token	*tokens;
// 	t_token	*last;
// 	char	*ptr;

// 	tokens = NULL;
// 	last = NULL;
// 	ptr = input;
// 	while (*ptr)
// 	{
// 		// Ignorer les espaces
// 		if (ft_isspace(*ptr))
// 		{
// 			ptr++;
// 			continue ;
// 		}
// 		// Identifier le type de token (pipe, redirection, mot)
// 		if (*ptr == '|')
// 		{
// 			add_token(&tokens, &last, create_token(ft_strdup("|"), PIPE));
// 			ptr++;
// 		}
// 		else if (*ptr == '<' && *(ptr + 1) == '<')
// 		{
// 			add_token(&tokens, &last, create_token(ft_strdup("<<"), HEREDOC));
// 			ptr += 2;
// 		}
// 		else if (*ptr == '>' && *(ptr + 1) == '>')
// 		{
// 			add_token(&tokens, &last, create_token(ft_strdup(">>"), APPEND));
// 			ptr += 2;
// 		}
// 		else if (*ptr == '<')
// 		{
// 			add_token(&tokens, &last, create_token(ft_strdup("<"), REDIR_IN));
// 			ptr++;
// 		}
// 		else if (*ptr == '>')
// 		{
// 			add_token(&tokens, &last, create_token(ft_strdup(">"), REDIR_OUT));
// 			ptr++;
// 		}
// 		else if (*ptr == '$')
// 			handle_env_var(&tokens, &last, &ptr);
// 		else if (*ptr == '\'' || *ptr == '"')
// 			handle_quotes(&tokens, &last, &ptr);
// 		else
// 			handle_word(&tokens, &last, &ptr);
// 		// Impression de débogage
// 		if (last)
// 			printf("Token ajouté: %s de type %d\n", last->content, last->type);
// 	}
// 	return (tokens);
// }

t_token	*lexer(char *input)
{
	t_token	*tokens;
	t_token	*last;
	char	*ptr;

	tokens = NULL;
	last = NULL;
	ptr = input;
	while (*ptr)
	{
		if (ft_isspace(*ptr))
		{
			ptr++;
			continue ;
		}
		else if (*ptr == '|')
		{
			add_token(&tokens, &last, create_token(ft_strdup("|"), PIPE));
			ptr++;
		}
		else if (*ptr == '<' && *(ptr + 1) == '<')
		{
			add_token(&tokens, &last, create_token(ft_strdup("<<"), HEREDOC));
			ptr += 2;
		}
		else if (*ptr == '>' && *(ptr + 1) == '>')
		{
			add_token(&tokens, &last, create_token(ft_strdup(">>"), APPEND));
			ptr += 2;
		}
		else if (*ptr == '<')
		{
			add_token(&tokens, &last, create_token(ft_strdup("<"), REDIR_IN));
			ptr++;
		}
		else if (*ptr == '>')
		{
			add_token(&tokens, &last, create_token(ft_strdup(">"), REDIR_OUT));
			ptr++;
		}
		else if (*ptr == '$')
			handle_env_var(&tokens, &last, &ptr);
		else if (*ptr == '\'' || *ptr == '"')
		{
			if (!handle_quotes(&tokens, &last, &ptr))
			{
				free_token(tokens);
				return (NULL);
			}
		}
		else
			handle_word(&tokens, &last, &ptr);
	}
	return (tokens);
}
