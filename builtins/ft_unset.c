/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:53 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 17:11:19 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"

// static int	is_valid_env_name(char *str)
// {
// 	int	i;

// 	if (!str || !(*str) || ft_isdigit(str[0]))
// 		return (0);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (!(ft_isalnum(str[i]) || str[i] == '_'))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// static char	**remove_env_var(char **old_env, char *var_name)
// {
// 	char	**new_env;
// 	int		len;
// 	int		i;
// 	int		j;
// 	int		found;

// 	len = 0;
// 	while (old_env[len])
// 		len++;
// 	new_env = malloc(sizeof(char *) * len);
// 	if (!new_env)
// 		return (NULL);

// 	i = 0;
// 	j = 0;
// 	found = 0;
// 	while (old_env[i])
// 	{
// 		if (ft_strncmp(old_env[i], var_name, ft_strlen(var_name)) == 0 
// 			&& old_env[i][ft_strlen(var_name)] == '=')
// 		{
// 			free(old_env[i]);
// 			found = 1;
// 		}
// 		else
// 			new_env[j++] = old_env[i];
// 		i++;
// 	}
// 	new_env[j] = NULL;
// 	if (!found)
// 		return (old_env);
// 	free(old_env);
// 	return (new_env);
// }

// int	ft_unset(char **args, char ***envp)
// {
// 	int	i;

// 	if (!args[1])
// 		return (0);

// 	i = 1;
// 	while (args[i])
// 	{
// 		if (!is_valid_env_name(args[i]))
// 		{
// 			ft_putstr_fd("minishell: unset: `", 2);
// 			ft_putstr_fd(args[i], 2);
// 			ft_putendl_fd("': not a valid identifier", 2);
// 			return (1);
// 		}

// 		*envp = remove_env_var(*envp, args[i]);
// 		if (!*envp)
// 		{
// 			ft_putendl_fd("minishell: unset: memory allocation error", 2);
// 			return (1);
// 		}
// 		i++;
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
    while (str[i])
    {
        if (!(ft_isalnum(str[i]) || str[i] == '_'))
            return (0);
        i++;
    }
    return (1);
}

static void remove_env_var(t_env **env, char *var_name)
{
    t_env *current = *env;
    t_env *prev = NULL;

    while (current)
    {
        if (ft_strcmp(current->key, var_name) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

int ft_unset(char **args, t_shell *shell)
{
    int i;

    if (!args[1])
        return (0);

    i = 1;
    while (args[i])
    {
        if (!is_valid_env_name(args[i]))
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putendl_fd("': not a valid identifier", 2);
            return (1);
        }
        remove_env_var(&shell->env, args[i]);
        i++;
    }
    return (0);
}
