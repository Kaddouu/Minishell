/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:11:57 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/28 16:03:17 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

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

// STRUCTURE POUR CHAQUE BUILTIN
typedef struct s_builtin
{
	char				*name;
	int					(*func)(t_shell *, char **);
}						t_builtin;

# include "./builtins/builtins.h"
# include "./parsing/parsing.h"

t_shell					*init_shell(char **envp);
t_env					*init_env(char **envp);
t_env					*create_env_node(char *key, char *value);
char					*extract_key(char *str, char *equals);
void					add_env_node(t_env **env_list, t_env *new);
t_env					*init_env(char **envp);
t_shell					*init_shell(char **envp);

void					free_token(t_token *token);
void					free_env(t_env *env);
void					free_shell(t_shell *shell);
void					free_command(t_command *cmd);
void					free_command_content(t_command *cmd);
char					*ft_strjoin_free(char *s1, char *s2);
char					*ft_strjoin_char_free(char *s, char c);

void					handle_eof(t_shell *shell);
void					setup_signals(void);
void					handle_sigint(int sig);

// Fonction d'exécution
// void					execute_commands(t_shell *shell);

// Fonctions utilitaires

#endif