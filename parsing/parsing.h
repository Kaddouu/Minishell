/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:40:26 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/04 15:51:49 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

t_command	*create_command(void);
void		add_command(t_command **cmds, t_command *new_cmd);
void		add_arg_to_command(t_command *cmd, char *arg);

t_env		*find_env_var(t_env *env, char *var_name);
void		expand_all_env_vars(t_token *tokens, t_env *env);

t_token		*create_token(char *content, t_type type);
void		add_token(t_token **tokens, t_token **last, t_token *new_token);
void		handle_env_var(t_token **tokens, t_token **last, char **ptr);
void		handle_word(t_token **tokens, t_token **last, char **ptr);
// void		handle_quotes(t_token **tokens, t_token **last, char **ptr);
int			handle_quotes(t_token **tokens, t_token **last, char **ptr);

t_token		*lexer(char *input);

t_command	*parser(t_token *tokens);
char		*expand_variables(char *str, t_env *env);
int			handle_input(t_shell *shell, char *line);

#endif
