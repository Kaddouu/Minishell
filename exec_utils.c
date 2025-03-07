/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:14:03 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 16:52:22 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_redirection(t_command *cmd)
{
	int	fd;
	int pipe_fd[2];

	// Redirection de sortie (>)
	if (cmd->output)
	{
		fd = open(cmd->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("Redirection failed: ", 2);
			ft_putstr_fd(cmd->output, 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	// Redirection en mode append (>>)
	if (cmd->append)
	{
		fd = open(cmd->append, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("Redirection failed: ", 2);
			ft_putstr_fd(cmd->append, 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	// Redirection d'entrée (<)
	if (cmd->input)
	{
		fd = open(cmd->input, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("Redirection failed: ", 2);
			ft_putstr_fd(cmd->input, 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	// Heredoc (<<)
	if (cmd->heredoc)
	{
		if (pipe(pipe_fd) == -1)
		{
			ft_putstr_fd("Pipe failed for heredoc\n", 2);
			exit(1);
		}
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	handle_heredoc(t_shell *shell, t_command *cmd)
{
	int		pipe_fd[2];
	char	*line;

	if (!cmd->heredoc)
		return ;
	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("Pipe failed for heredoc\n", 2);
		shell->exit_status = 1;
		return ;
	}
	while ((line = readline("> ")) && ft_strcmp(line, cmd->heredoc) != 0)
	{
		if (cmd->expand_heredoc)
			line = expand_variables(line, shell->env);
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putchar_fd('\n', pipe_fd[1]);
		free(line);
	}
	free(line);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}

char	*find_path(char *cmd, t_env *env)
{
	t_env	*path;
	char	**dirs;
	int		i;
	char	*full_path;

	path = find_env_var(env, "PATH");
	if (!path)
		return (ft_strdup(cmd));
	dirs = ft_split(path->value, ':');
	i = 0;
	while (dirs[i])
	{
		full_path = ft_strjoin_three(dirs[i], "/", cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(dirs);
	return (ft_strdup(cmd));
}

char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*current;
	char	**env_array;
	int		i;

	// Compter le nombre de variables d'environnement
	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	// Remplir le tableau avec "key=value"
	current = env;
	i = 0;
	while (current)
	{
		env_array[i] = ft_strjoin_three(current->key, "=", current->value);
		if (!env_array[i])
		{
			free_split(env_array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
