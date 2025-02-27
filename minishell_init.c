/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:17:11 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/14 19:01:20 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_node(char *key, char *value)
{
	t_env	*new;

	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

char	*extract_key(char *str, char *equals)
{
	char	*key;

	key = (char *)ft_calloc(equals - str + 1, sizeof(char));
	if (!key)
		return (NULL);
	ft_strlcpy(key, str, equals - str + 1);
	return (key);
}

void	add_env_node(t_env **env_list, t_env *new)
{
	if (!new)
		return ;
	new->next = *env_list;
	*env_list = new;
}

// INITIALISATION DE L'ENVIRONNEMENT
t_env	*init_env(char **envp)
{
	t_env	*env_list;
	char	*equals;
	char	*key;
	char	*value;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equals = ft_strchr(envp[i], '=');
		if (equals)
		{
			key = extract_key(envp[i], equals);
			value = ft_strdup(equals + 1);
			if (!key || !value)
			{
				free(key);
				free(value);
				return (NULL);
			}
			add_env_node(&env_list, create_env_node(key, value));
		}
		i++;
	}
	return (env_list);
}

// INITIALISATION DE LA STRUCTURE PRINCIPALE DU SHELL
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env = init_env(envp);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->running = 1;
	return (shell);
}
