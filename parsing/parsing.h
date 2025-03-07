/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:40:26 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 17:32:03 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

typedef enum e_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	ENV
}						t_type;

// GESTION DES VARIABLES D'ENVIRONNEMENT TEL QUE ENV, EXPORT...
typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_token
{
	char				*content;
	enum e_type			type;
	struct s_token		*next;
}						t_token;

typedef struct s_command
{
	char				**args;
	char				*input;
	char				*output;
	char				*heredoc;
	char				*append;
	int					expand_heredoc; // 1 pour expanser, 0 sinon
	struct s_command	*next;
}						t_command;

// STRUCTURE CENTRALE DU SHELL
typedef struct s_shell
{
	t_env				*env;
	t_command			*cmds;
	t_token				*tokens;
	char				*last_cmd;
	int					exit_status;
	int					running;
}						t_shell;

// Fonctions utilitaires
char		*expand_variables(char *str, t_env *env);
t_env		*find_env_var(t_env *env, char *var_name);
void		expand_all_env_vars(t_token *tokens, t_env *env);

// Fonctions handling
int			handle_input(t_shell *shell, char *line);
void		handle_word(t_token **tokens, t_token **last, char **ptr);
int			handle_quotes(t_token **tokens, t_token **last, char **ptr);
void		handle_env_var(t_token **tokens, t_token **last, char **ptr);

// Fonctions de tokenisation
t_token		*lexer(char *input);
t_token		*create_token(char *content, t_type type);
void		add_token(t_token **tokens, t_token **last, t_token *new_token);

// Fonctions de parsing
t_command	*parser(t_token *tokens);
t_command	*create_command(void);
void		add_arg_to_command(t_command *cmd, char *arg);
void		add_command(t_command **cmds, t_command *new_cmd);

#endif
