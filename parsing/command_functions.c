/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:36 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/11 11:55:40 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->heredoc = NULL;
	cmd->expand_heredoc = 1;
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
		*cmds = new_cmd;
	else
	{
		current = *cmds;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
}

void	add_arg_to_command(t_command *cmd, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	if (!cmd || !arg)
		return ;
	while (cmd->args && cmd->args[i])
		i++;
	new_args = realloc(cmd->args, sizeof(char *) * (i + 2));
	if (!new_args)
	{
		free_command_content(cmd);
		return ;
	}
	cmd->args = new_args;
	cmd->args[i] = ft_strdup(arg);
	if (!cmd->args[i])
	{
		free_command_content(cmd);
		return ;
	}
	cmd->args[i + 1] = NULL;
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	*ptr;
	char	*start;
	char	*var_name;
	t_env	*env_var;
	char	*temp;

	if (!str || !env)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
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
			if (!var_name)
			{
				free(result);
				return (NULL);
			}
			env_var = find_env_var(env, var_name);
			if (env_var)
			{
				temp = ft_strjoin_free(result, env_var->value);
				if (!temp)
				{
					free(var_name);
					free(result);
					return (NULL);
				}
				result = temp;
			}
			free(var_name);
		}
		else
		{
			temp = ft_strjoin_char_free(result, *ptr);
			if (!temp)
			{
				free(result);
				return (NULL);
			}
			result = temp;
			ptr++;
		}
	}
	return (result);
}
