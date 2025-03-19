/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:26:20 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/19 15:30:33 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_env_vars(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	free_env_array_on_error(char **env_array, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(env_array[j]);
		j++;
	}
	free(env_array);
}

char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*current;
	char	**env_array;
	int		i;

	count = count_env_vars(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		env_array[i] = ft_strjoin_three(current->key, "=", current->value);
		if (!env_array[i])
		{
			free_env_array_on_error(env_array, i);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
