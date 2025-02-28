/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:36 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/28 15:42:30 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);

	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->heredoc = NULL;
	cmd->append = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command(t_command **cmds, t_command *new_cmd)
{
	t_command	*current;

	if (!new_cmd)
		return ;
	if (!*cmds)
	{
		*cmds = new_cmd;
	}
	else
	{
		current = *cmds;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
}

void add_arg_to_command(t_command *cmd, char *arg) {
    if (!cmd || !arg) return; // Vérifiez que cmd et arg ne sont pas NULL

    // Trouver la taille actuelle des arguments
    int i = 0;
    while (cmd->args && cmd->args[i]) i++;

    // Réallouer de la mémoire pour ajouter un nouvel argument
    cmd->args = realloc(cmd->args, sizeof(char *) * (i + 2));
    if (!cmd->args) {
        // Gérer l'erreur d'allocation
        return;
    }
    cmd->args[i] = ft_strdup(arg); // Dupliquer l'argument
    cmd->args[i + 1] = NULL; // Terminer le tableau avec NULL
}
