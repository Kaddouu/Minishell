/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:30:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/20 16:06:06 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_lexer_context(t_lexer_context *ctx, t_token **tokens,
		t_token **last, char **ptr)
{
	ctx->tokens = tokens;
	ctx->last = last;
	ctx->ptr = ptr;
}

// In lexer.c
t_token	*lexer(char *input, t_env *env, int exit_status)
{
	t_token			*tokens;
	t_token			*last;
	char			*ptr;
	t_lexer_context	ctx;

	tokens = NULL;
	last = NULL;
	ptr = input;
	init_lexer_context(&ctx, &tokens, &last, &ptr);
	ctx.env = env;
	ctx.exit_status = exit_status;
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
		else
			handle_argument(&ctx);
	}
	return (tokens);
}
