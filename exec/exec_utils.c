/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:14:03 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/19 15:25:18 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd, t_builtin *builtins)
{
	int	i;

	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd, builtins[i].name) == 0)
			return (1);
		i++;
	}
	return (0);
}

char	*find_path(char *cmd, t_env *env)
{
	t_env	*path;
	char	**dirs;
	char	*full_path;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = find_env_var(env, "PATH");
	if (!path)
		return (ft_strdup(cmd));
	dirs = ft_split(path->value, ':');
	if (!dirs)
		return (ft_strdup(cmd));
	full_path = check_path_dirs(dirs, cmd);
	free_split(dirs);
	return (full_path);
}

char	*check_path_dirs(char **dirs, char *cmd)
{
	int		i;
	char	*full_path;

	i = 0;
	while (dirs[i])
	{
		full_path = ft_strjoin_three(dirs[i], "/", cmd);
		if (!full_path)
			return (ft_strdup(cmd));
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (ft_strdup(cmd));
}
