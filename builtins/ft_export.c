/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 14:32:53 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	if (!new_node->key)
		return ;
	new_node->value = ft_strdup(value);
	if (!new_node->value)
		return ;
	new_node->next = *env;
	*env = new_node;
}

static int is_valid_env_name(char *str)
{
    int i;

    if (!str || !str[0] || ft_isdigit(str[0])) // Key can't start with digit or be empty
        return (0);
    i = 0;
    while (str[i] && str[i] != '=') // Only validate up to '='
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int ft_export(char **args, t_shell *shell)
{
    int i;
    char *equals;
    char *key;
    char *value;
    t_env *current;
    int ret;

    if (!args[1])
    {
        current = shell->env;
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
    ret = 0;
    while (args[i])
    {
        equals = ft_strchr(args[i], '=');
        if (equals)
        {
            key = ft_substr(args[i], 0, equals - args[i]);
            value = ft_strdup(equals + 1); // Value can be anything, including $
            if (!is_valid_env_name(key)) // Only validate key
            {
                ft_putstr_fd("minishell: export: `", 2);
                ft_putstr_fd(key, 2);
                ft_putendl_fd("': not a valid identifier", 2);
                ret = 1;
                free(key);
                free(value);
            }
            else
            {
                // Remove quotes from value if present (optional, for consistency)
                if (value[0] == '"' && value[ft_strlen(value) - 1] == '"')
                {
                    char *temp = ft_substr(value, 1, ft_strlen(value) - 2);
                    free(value);
                    value = temp;
                }
                add_or_update_env(&shell->env, key, value);
                free(key);
                free(value);
            }
        }
        else
        {
            if (!is_valid_env_name(args[i]))
            {
                ft_putstr_fd("minishell: export: `", 2);
                ft_putstr_fd(args[i], 2);
                ft_putendl_fd("': not a valid identifier", 2);
                ret = 1;
            }
            else
            {
                current = shell->env;
                while (current && ft_strcmp(current->key, args[i]) != 0)
                    current = current->next;
                if (!current)
                    add_or_update_env(&shell->env, args[i], "");
            }
        }
        i++;
    }
    return (ret);
}
