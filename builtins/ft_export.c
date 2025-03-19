/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/19 13:49:08 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_env_name(char *str)
{
	int	i;

	if (!str || !str[0] || ft_isdigit(str[0]))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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

static int	handle_export_with_assignment(t_shell *shell, char *arg)
{
	char	*equals;
	char	*key;
	char	*value;
	char	*temp;

	equals = ft_strchr(arg, '=');
	key = ft_substr(arg, 0, equals - arg);
	value = ft_strdup(equals + 1);
	if (!is_valid_env_name(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(key, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (free(key), free(value), 1);
	}
	if (value[0] == '"' && value[ft_strlen(value) - 1] == '"')
	{
		temp = ft_substr(value, 1, ft_strlen(value) - 2);
		free(value);
		value = temp;
	}
	add_or_update_env(&shell->env, key, value);
	return (free(key), free(value), 0);
}

static int	handle_export_without_assignment(t_shell *shell, char *arg)
{
	t_env	*current;

	if (!is_valid_env_name(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	current = shell->env;
	while (current && ft_strcmp(current->key, arg) != 0)
		current = current->next;
	if (!current)
		add_or_update_env(&shell->env, arg, "");
	return (0);
}

int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	ret;
	int	status;

	if (!args[1])
	{
		print_sorted_env(shell->env);
		return (0);
	}
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			status = handle_export_with_assignment(shell, args[i]);
		else
			status = handle_export_without_assignment(shell, args[i]);
		if (status == 1)
			ret = 1;
		i++;
	}
	return (ret);
}
