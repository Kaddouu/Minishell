/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:52:11 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/19 09:36:01 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input(t_shell *shell, char *line)
{
	if (!line || !shell)
		return (-1);
	if (ft_strcmp(line, "exit") == 0)
		return (-1);
	shell->tokens = lexer(line, shell->env, shell->exit_status);
	if (!shell->tokens)
		return (0);
	expand_all_env_vars(shell->tokens, shell->env, shell->exit_status);
	shell->cmds = parser(shell->tokens);
	if (!shell->cmds)
	{
		free_token(shell->tokens);
		shell->tokens = NULL;
		return (0);
	}
	free_token(shell->tokens);
	shell->tokens = NULL;
	free_command(shell->cmds);
	shell->cmds = NULL;
	return (0);
}

t_command	*parser(t_token *tokens)
{
	t_command	*cmds;
	t_command	*current_cmd;
	t_token		*current;
	char		*delimiter;
	char		quote;
	size_t		len;
	char		*new_delimiter;

	cmds = NULL;
	current_cmd = NULL;
	current = tokens;
	if (!tokens)
		return (NULL);
	while (current)
	{
		if (!current_cmd)
		{
			current_cmd = create_command();
			if (!current_cmd)
			{
				free_command(cmds);
				return (NULL);
			}
			add_command(&cmds, current_cmd);
		}
		if (current->type == PIPE)
		{
			if (!current_cmd->args || (!current_cmd->args[0]
					&& !current_cmd->input && !current_cmd->output
					&& !current_cmd->heredoc && !current_cmd->append))
			{
				printf(" syntax error near unexpected token `|'\n");
				free_command(cmds);
				return (NULL);
			}
			current = current->next;
			if (!current || current->type == PIPE)
			{
				printf("Erreur : pipe suivi d'un pipe ou fin de ligne\n");
				free_command(cmds);
				return (NULL);
			}
			current_cmd = create_command();
			if (!current_cmd)
			{
				free_command(cmds);
				return (NULL);
			}
			add_command(&cmds, current_cmd);
		}
		else if (current->type == REDIR_IN)
		{
			current = current->next;
			if (!current || current->type != WORD)
			{
				// printf("Erreur : redirection d'entrée sans fichier\n");
				free_command(cmds);
				return (NULL);
			}
			current_cmd->input = ft_strdup(current->content);
			if (!current_cmd->input)
			{
				free_command(cmds);
				return (NULL);
			}
			current = current->next;
		}
		else if (current->type == REDIR_OUT)
		{
			current = current->next;
			if (!current || current->type != WORD)
			{
				printf("Erreur : redirection de sortie sans fichier\n");
				free_command(cmds);
				return (NULL);
			}
			current_cmd->output = ft_strdup(current->content);
			if (!current_cmd->output)
			{
				free_command(cmds);
				return (NULL);
			}
			current = current->next;
		}
		else if (current->type == HEREDOC)
		{
			current = current->next;
			if (!current || current->type != WORD)
			{
				printf("Erreur : heredoc sans délimiteur\n");
				free_command(cmds);
				return (NULL);
			}
			
			// Libérer le heredoc précédent s'il existe
			if (current_cmd->heredoc)
				free(current_cmd->heredoc);
			
			delimiter = ft_strdup(current->content);
			if (!delimiter)
			{
				free_command(cmds);
				return (NULL);
			}
			
			// Traitement des guillemets et expansion
			if (delimiter[0] == '\'' || delimiter[0] == '"')
			{
				quote = delimiter[0];
				len = ft_strlen(delimiter);
				if (len > 1 && delimiter[len - 1] == quote)
				{
					new_delimiter = ft_substr(delimiter, 1, len - 2);
					free(delimiter);
					if (!new_delimiter)
					{
						free_command(cmds);
						return (NULL);
					}
					delimiter = new_delimiter;
					current_cmd->expand_heredoc = 0;
				}
				else
				{
					printf("Erreur : guillemet non fermé dans le délimiteur du heredoc\n");
					free(delimiter);
					free_command(cmds);
					return (NULL);
				}
			}
			else
			{
				current_cmd->expand_heredoc = 1;
			}
			current_cmd->heredoc = delimiter;
			current = current->next;
		}
		else if (current->type == APPEND)
		{
			current = current->next;
			if (!current || current->type != WORD)
			{
				printf("Erreur : redirection en append sans fichier\n");
				free_command(cmds);
				return (NULL);
			}
			current_cmd->append = ft_strdup(current->content);
			if (!current_cmd->append)
			{
				free_command(cmds);
				return (NULL);
			}
			current = current->next;
		}
		else if (current->type == WORD || current->type == ENV)
		{
			add_arg_to_command(current_cmd, current->content);
			current = current->next;
		}
		else
		{
			printf("Erreur : type de token inattendu\n");
			free_command(cmds);
			return (NULL);
		}
	}
	return (cmds);
}
