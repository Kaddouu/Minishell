/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:11:57 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 17:21:43 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./Libft/libft.h"
# include "./builtins/builtins.h"
# include "./parsing/parsing.h"
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_shell_state;

// Fonction d'initialisation
t_builtin						*init_builtins(void);
t_env							*init_env(char **envp);
t_shell							*init_shell(char **envp);

// Fonction d'exécution
// void		execute_commands(t_shell *shell, t_builtin *builtins);
void							execute_commands(t_shell *shell,
									t_builtin *builtins);
void							execute_pipeline(t_shell *shell,
									t_builtin *builtins);
void							execute_external(t_shell *shell, t_command *cmd,
									t_env *env, int heredoc_fd);
// void		execute_external(t_shell *shell, t_command *cmd, t_env *env);
int								execute_builtin(t_shell *shell, t_command *cmd,
									t_builtin *builtins);

// Fonctions utilitaires
int								is_builtin(char *cmd, t_builtin *builtins);
char							*find_path(char *cmd, t_env *env);
t_env							*find_env_var(t_env *env, char *key);
char							**env_to_array(t_env *env);
void							setup_signals(void);

// Fonctions de libération de mémoire
void							free_env(t_env *env);
void							free_split(char **split);
void							free_shell(t_shell *shell);
void							free_token(t_token *token);
void							free_command(t_command *cmd);
void							free_builtins(t_builtin *builtins);
void							free_command_content(t_command *cmd);

// Fonctions handling
void							handle_sigint(int sig);
void							handle_eof(t_shell *shell);
// void		handle_redirection(t_command *cmd);
void							handle_redirection(t_command *cmd,
									int heredoc_fd);
// void		handle_heredoc(t_shell *shell, t_command *cmd);
int								handle_heredoc(t_shell *shell, t_command *cmd);

#endif