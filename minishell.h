/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:11:57 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/26 17:48:58 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./Libft/libft.h"
# include "./builtins/builtins.h"
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

// typedef struct s_token
// {
//     char            *content;   // Contenu du token
//     // enum e_type     type;      // WORD, PIPE, REDIR_IN, REDIR_OUT, etc.
//     struct s_token  *next;
// } t_token;

// typedef struct s_command
// {
//     char            **args;     // Tableau d'arguments de la commande
//     char            *input;     // Fichier d'entrée (<)
//     char            *output;    // Fichier de sortie (>)
//     char            *heredoc;   // Heredoc (<<)
//     char            *append;    // Append (>>)
//     struct s_command *next;     // Commande suivante après pipe
// } t_command;

// typedef struct s_env
// {
//     char        *key;          // Nom de la variable
//     char        *value;        // Valeur de la variable
//     struct s_env *next;
// } t_env;

// typedef struct s_shell
// {
//     t_env       *env;          // Liste des variables d'environnement
//     t_command   *cmds;         // Liste des commandes à exécuter
//     t_token     *tokens;       // Liste des tokens après lexing
//     char        *last_cmd;     // Dernière commande pour '!'
//     int         exit_status;   // Code de retour de la dernière commande
//     int         running;       // Shell en cours d'exécution
// } t_shell;

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

t_shell					*init_shell(char **envp);
t_env					*init_env(char **envp);
void					free_shell(t_shell *shell);

char					**parse_command(const char *command);
void					free_result(char **result, size_t i);

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
int						handle_input(t_shell *shell, char *line);

void					handle_eof(t_shell *shell);
void					setup_signals(void);
void					handle_sigint(int sig);

// Fonctions pour le lexer
t_token					*lexer(char *input);
t_token					*create_token(char *content, t_type type);
void					add_token(t_token **tokens, t_token **last,
							t_token *new_token);
void					handle_quotes(t_token **tokens, t_token **last,
							char **ptr);
void					handle_env_var(t_token **tokens, t_token **last,
							char **ptr);
void					handle_word(t_token **tokens, t_token **last,
							char **ptr);

// Fonctions pour le parser
t_command				*parser(t_token *tokens);
t_command				*create_command(void);
void					add_command(t_command **cmds, t_command *new_cmd);
void					add_arg_to_command(t_command *cmd, char *arg);
char					*expand_variables(char *str, t_env *env);
t_env					*find_env_var(t_env *env, char *var_name);
void					expand_all_env_vars(t_token *tokens, t_env *env);

// Fonction d'exécution
void					execute_commands(t_shell *shell);

// Fonctions utilitaires
char					*ft_strjoin_free(char *s1, char *s2);
char					*ft_strjoin_char_free(char *s, char c);

#endif