/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:52:11 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/28 15:59:09 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// t_command *parser(t_token *tokens) {
//     t_command *cmds = NULL;
//     t_command *current_cmd = NULL;
//     t_token *current = tokens;

//     while (current) {
//         if (!current_cmd) {
//             current_cmd = create_command();
//             if (!current_cmd) {
//                 // Gérer l'erreur d'allocation
//                 return NULL;
//             }
//             add_command(&cmds, current_cmd);
//         }
//         if (current->type == PIPE) {
//             current_cmd = create_command();
//             if (!current_cmd) {
//                 // Gérer l'erreur d'allocation
//                 return NULL;
//             }
//             add_command(&cmds, current_cmd);
//         } else if (current->type == REDIR_IN) {
//             current = current->next;
//             if (current && current->type == WORD) {
//                 current_cmd->input = ft_strdup(current->content);
//             }
//         } else if (current->type == REDIR_OUT) {
//             current = current->next;
//             if (current && current->type == WORD) {
//                 current_cmd->output = ft_strdup(current->content);
//             }
//         } else if (current->type == WORD || current->type == ENV) {
//             add_arg_to_command(current_cmd, current->content);
//         }
//         current = current->next;
//     }
//     return cmds;
// }

t_command *parser(t_token *tokens) {
    t_command *cmds = NULL;
    t_command *current_cmd = NULL;
    t_token *current = tokens;

    while (current) {
        if (!current_cmd) {
            current_cmd = create_command();
            if (!current_cmd) return NULL;
            add_command(&cmds, current_cmd);
        }
        if (current->type == PIPE) {
            if (!current_cmd->args[0] && !current_cmd->input && !current_cmd->output && !current_cmd->heredoc) {
                free_command(cmds);
                return NULL;
            }
            t_token *next = current->next;
            if (!next || next->type == PIPE) {
                free_command(cmds);
                return NULL;
            }
            current_cmd = create_command();
            if (!current_cmd) return NULL;
            add_command(&cmds, current_cmd);
        } else if (current->type == REDIR_IN) {
            current = current->next;
            if (current && current->type == WORD) {
                current_cmd->input = ft_strdup(current->content);
            }
        } else if (current->type == REDIR_OUT) {
            current = current->next;
            if (current && current->type == WORD) {
                current_cmd->output = ft_strdup(current->content);
            }
        } else if (current->type == HEREDOC) {
            current = current->next;
            if (current && current->type == WORD) {
                current_cmd->heredoc = ft_strdup(current->content);
            }
        } else if (current->type == WORD || current->type == ENV) {
            add_arg_to_command(current_cmd, current->content);
        }
        current = current->next;
    }
    return cmds;
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	*ptr;
	char	*start;
	char	*var_name;
	t_env	*env_var;

	result = ft_strdup("");
	ptr = str;
	while (*ptr)
	{
		if (*ptr == '$' && *(ptr + 1) && (ft_isalnum(*(ptr + 1)) || *(ptr
					+ 1) == '_'))
		{
			ptr++;
			start = ptr;
			while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
				ptr++;
			var_name = ft_substr(start, 0, ptr - start);
			env_var = find_env_var(env, var_name);
			if (env_var)
				result = ft_strjoin_free(result, env_var->value);
			free(var_name);
		}
		else
		{
			result = ft_strjoin_char_free(result, *ptr);
			ptr++;
		}
	}
	return (result);
}

int	handle_input(t_shell *shell, char *line)
{
	if (!line || !shell)
		return (-1);
	if (ft_strcmp(line, "exit") == 0)
		return (-1);
	// Analyse lexicale
	shell->tokens = lexer(line);
	if (!shell->tokens)
		return (0); // Ligne vide ou erreur
	// Expansion des variables d'environnement
	expand_all_env_vars(shell->tokens, shell->env);
	// Analyse syntaxique
	shell->cmds = parser(shell->tokens);
	if (!shell->cmds)
	{
		free_token(shell->tokens);
		shell->tokens = NULL;
		return (0); // Erreur de parsing
	}
	// Exécution des commandes
	// execute_commands(shell);
	// Nettoyage
	free_token(shell->tokens);
	shell->tokens = NULL;
	free_command(shell->cmds);
	shell->cmds = NULL;
	return (0);
}
