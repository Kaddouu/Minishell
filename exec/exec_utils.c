/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:14:03 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 18:56:49 by ilkaddou         ###   ########.fr       */
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

void handle_redirection(t_command *cmd, int heredoc_fd)
{
    int fd;

    // Only set input redirection if we don't have a heredoc or heredoc_fd is -1
    if (cmd->input && heredoc_fd == -1)
    {
        fd = open(cmd->input, O_RDONLY);
        if (fd < 0)
        {
            ft_putstr_fd("Redirection failed: Cannot open ", 2);
            ft_putstr_fd(cmd->input, 2);
            ft_putstr_fd("\n", 2);
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    else if (heredoc_fd != -1)
    {
        // If heredoc_fd is provided, use it as standard input
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
    }

    // Handle output redirection
    if (cmd->output)
    {
        fd = open(cmd->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            ft_putstr_fd("Redirection failed: Cannot open ", 2);
            ft_putstr_fd(cmd->output, 2);
            ft_putstr_fd("\n", 2);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    
    // Handle append redirection (has precedence over normal output redirection)
    if (cmd->append)
    {
        fd = open(cmd->append, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
        {
            ft_putstr_fd("Redirection failed: Cannot open ", 2);
            ft_putstr_fd(cmd->append, 2);
            ft_putstr_fd("\n", 2);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

int handle_heredoc(t_shell *shell, t_command *cmd)
{
    int pipe_fd[2];
    char *line;
    char *expanded_line;
    int found_last_delimiter = 0; // Nouveau flag

    if (!cmd->heredoc)
        return (-1);

    if (pipe(pipe_fd) == -1)
    {
        ft_putstr_fd("Pipe failed for heredoc\n", 2);
        shell->exit_status = 1;
        return (-1);
    }

    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            ft_putstr_fd("warning: here-document delimited by end-of-file\n", 2);
            break;
        }

        // Vérifier si la ligne est le dernier délimiteur
        if (ft_strcmp(line, cmd->heredoc) == 0)
        {
            free(line);
            break;
        }

        // Si le dernier délimiteur n'a pas été trouvé, ignorer les lignes
        if (!found_last_delimiter)
        {
            free(line);
            continue;
        }

        // Traitement de la ligne (expansion des variables)
        if (cmd->expand_heredoc)
        {
            expanded_line = expand_variables(line, shell->env);
            if (expanded_line)
            {
                ft_putstr_fd(expanded_line, pipe_fd[1]);
                ft_putchar_fd('\n', pipe_fd[1]);
                free(expanded_line);
            }
        }
        else
        {
            ft_putstr_fd(line, pipe_fd[1]);
            ft_putchar_fd('\n', pipe_fd[1]);
        }

        free(line);
    }

    close(pipe_fd[1]);
    return (pipe_fd[0]);
}

char	*find_path(char *cmd, t_env *env)
{
	t_env	*path;
	char	**dirs;
	int		i;
	char	*full_path;

	// If command contains a slash, it's a direct path
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));

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
