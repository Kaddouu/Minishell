/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:30:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 14:14:35 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// In lexer.c
t_token *lexer(char *input, t_env *env, int exit_status)
{
    t_token *tokens = NULL;
    t_token *last = NULL;
    char *ptr = input;
    while (*ptr)
    {
        if (ft_isspace(*ptr))
        {
            ptr++;
            continue;
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
            handle_argument(&tokens, &last, &ptr, env, exit_status);
    }
    return (tokens);
}
