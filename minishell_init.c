/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:17:11 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/11 11:47:33 by ilkaddou         ###   ########.fr       */
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

t_env	*init_env(char **ev)
{
	t_env	*env_list;
	char	*equals;
	char	*key;
	char	*value;
	int		i;

	env_list = NULL;
	i = 0;
	while (ev[i])
	{
		equals = ft_strchr(ev[i], '=');
		if (equals)
		{
			key = extract_key(ev[i], equals);
			value = ft_strdup(equals + 1);
			if (!key || !value)
				return (free(key), free(value), NULL);
			add_env_node(&env_list, create_env_node(key, value));
		}
		i++;
	}
	return (env_list);
}

t_env	*init_min_env(void)
{
	t_env	*env;

	env = create_env_node("PWD", getcwd(NULL, 0));
	add_env_node(&env, create_env_node("SHLVL", "1"));
	add_env_node(&env, create_env_node("_", "/usr/bin/env"));
	add_env_node(&env, create_env_node("PATH",
			"/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"));
	return (env);
}

t_shell	*init_shell(char **ev)
{
	t_shell	*shell;

	shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (NULL);
	if (ev)
		shell->env = init_env(ev);
	else
		shell->env = init_min_env();
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->running = 1;
	return (shell);
}

t_builtin	*init_builtins(void)
{
	t_builtin	*builtins;

	builtins = malloc(sizeof(t_builtin) * 8);
	if (!builtins)
		return (NULL);
	builtins[0].name = "cd";
	builtins[0].func = ft_cd;
	builtins[1].name = "echo";
	builtins[1].func = ft_echo;
	builtins[2].name = "env";
	builtins[2].func = ft_env;
	builtins[3].name = "exit";
	builtins[3].func = ft_exit;
	builtins[4].name = "export";
	builtins[4].func = ft_export;
	builtins[5].name = "pwd";
	builtins[5].func = ft_pwd;
	builtins[6].name = "unset";
	builtins[6].func = ft_unset;
	builtins[7].name = NULL;
	builtins[7].func = NULL;
	return (builtins);
}
