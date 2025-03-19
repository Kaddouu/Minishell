/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:18:27 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/19 13:49:25 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*copy_env_list(t_env *env)
{
	t_env	*new_list;
	t_env	*new_node;
	t_env	*current;
	t_env	*last;

	new_list = NULL;
	current = env;
	last = NULL;
	while (current)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = strdup(current->key);
		new_node->value = strdup(current->value);
		new_node->next = NULL;
		if (!new_list)
			new_list = new_node;
		else
			last->next = new_node;
		last = new_node;
		current = current->next;
	}
	return (new_list);
}

void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

void	sort_env(t_env *env_list)
{
	t_env	*current;
	int		swapped;

	swapped = 1;
	if (!env_list)
		return ;
	while (swapped)
	{
		swapped = 0;
		current = env_list;
		while (current->next)
		{
			if (strcmp(current->key, current->next->key) > 0)
			{
				swap_env_nodes(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
	}
}

void	print_sorted_env(t_env *env)
{
	t_env	*sorted_env;
	t_env	*current;

	sorted_env = copy_env_list(env);
	if (!sorted_env)
		return ;
	sort_env(sorted_env);
	current = sorted_env;
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
	free_env_list(sorted_env);
}

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
