/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 18:03:06 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"

// static int	is_valid_env_name(char *str)
// {
// 	int	i;

// 	if (!str || !(*str) || ft_isdigit(str[0]))
// 		return (0);
// 	i = 0;
// 	while (str[i] && str[i] != '=')
// 	{
// 		if (!(ft_isalnum(str[i]) || str[i] == '_'))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// static char	**extend_env(char **old_env, char *new_var)
// {
// 	char	**new_env;
// 	int		len;
// 	int		i;

// 	len = 0;
// 	while (old_env[len])
// 		len++;
// 	new_env = malloc(sizeof(char *) * (len + 2));
// 	if (!new_env)
// 		return (NULL);
// 	i = 0;
// 	while (i < len)
// 	{
// 		new_env[i] = old_env[i];
// 		i++;
// 	}
// 	new_env[i] = ft_strdup(new_var);
// 	new_env[i + 1] = NULL;
// 	free(old_env);
// 	return (new_env);
// }

// int	ft_export(char **args, char ***envp)
// {
// 	int	i;

// 	if (!args[1])
// 	{
// 		i = 0;
// 		while ((*envp)[i++])
// 		{
// 			ft_putstr_fd("declare -x ", 1);
// 			ft_putendl_fd((*envp)[i], 1);
// 		}
// 		return (0);
// 	}
// 	i = 1;
// 	while (args[i])
// 	{
// 		if (!is_valid_env_name(args[i]))
// 		{
// 			ft_putstr_fd("minishell: export: `", 2);
// 			ft_putstr_fd(args[i], 2);
// 			ft_putendl_fd("': not a valid identifier", 2);
// 			return (1);
// 		}
// 		*envp = extend_env(*envp, args[i++]);
// 		if (!*envp)
// 		{
// 			ft_putendl_fd("minishell: export: memory allocation error", 2);
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

#include "../minishell.h"

static int is_valid_env_name(char *str)
{
    int i;

    if (!str || !(*str) || ft_isdigit(str[0]))
        return (0);
    i = 0;
    while (str[i] && str[i] != '=')
    {
        if (!(ft_isalnum(str[i]) || str[i] == '_'))
            return (0);
        i++;
    }
    return (1);
}

static void add_or_update_env(t_env **env, char *key, char *value)
{
    t_env *current = *env;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = *env;
    *env = new_node;
}

int ft_export(char **args, t_shell *shell)
{
    int i;
    char *equals;
    char *key;
    char *value;

    if (!args[1])
    {
        t_env *current = shell->env;
        while (current)
        {
            ft_putstr_fd("declare -x ", 1);
            ft_putstr_fd(current->key, 1);
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(current->value, 1);
            ft_putendl_fd("\"", 1);
            current = current->next;
        }
        return (0);
    }
    i = 1;
    while (args[i])
    {
        if (!is_valid_env_name(args[i]))
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putendl_fd("': not a valid identifier", 2);
            return (1);
        }

        equals = ft_strchr(args[i], '=');
        if (equals)
        {
            key = ft_substr(args[i], 0, equals - args[i]);
            value = ft_strdup(equals + 1);
            add_or_update_env(&shell->env, key, value);
            free(key);
            free(value);
        }
        else
            add_or_update_env(&shell->env, args[i], "");
        i++;
    }
    return (0);
}
