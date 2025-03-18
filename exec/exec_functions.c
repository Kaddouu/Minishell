/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:52:16 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/18 10:37:27 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(t_shell *shell, t_command *cmd, t_builtin *builtins)
{
	int	i;

	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd->args[0], builtins[i].name) == 0)
		{
			if (ft_strcmp(cmd->args[0], "export") == 0)
				return (ft_export(cmd->args, shell));
			else if (ft_strcmp(cmd->args[0], "unset") == 0)
				return (ft_unset(cmd->args, shell));
			else if (ft_strcmp(cmd->args[0], "env") == 0)
				return (builtins[i].func(env_to_array(shell->env), shell));
			else
				return (builtins[i].func(cmd->args, shell));
		}
		i++;
	}
	return (-1);
}

void execute_external(t_shell *shell, t_command *cmd, t_env *env, int heredoc_fd)
{
    pid_t   pid;
    char    *path;

    g_shell_state = 1;
    
    pid = fork();
    if (pid == -1)
    {
        ft_putstr_fd("Fork failed\n", 2);
        shell->exit_status = 1;
        g_shell_state = 0;
        return;
    }
    if (pid == 0)
    {
        handle_redirection(cmd, heredoc_fd);
        path = find_path(cmd->args[0], env);
        if (!path || access(path, X_OK) != 0)
        {
            ft_putstr_fd("Command not found: ", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd("\n", 2);
            free(path);
            exit(127);
        }
        execve(path, cmd->args, env_to_array(env));
        ft_putstr_fd("Execve failed: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd("\n", 2);
        free(path);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (heredoc_fd != -1)
            close(heredoc_fd);
        waitpid(pid, &shell->exit_status, 0);
        if (WIFEXITED(shell->exit_status))
            shell->exit_status = WEXITSTATUS(shell->exit_status);
        if (g_shell_state == 2)
            g_shell_state = 0;
        else
            g_shell_state = 0;
    }
}

void execute_pipeline(t_shell *shell, t_builtin *builtins)
{
    t_command   *cmd;
    int         pipe_fd[2];
    int         prev_fd;
    pid_t       *pids;
    int         cmd_count;
    int         i;
    int         status;
    char        *path;
    int         heredoc_fd;

    cmd = shell->cmds;
    prev_fd = STDIN_FILENO;
    cmd_count = 0;
    t_command *tmp = shell->cmds;
    while (tmp)
    {
        cmd_count++;
        tmp = tmp->next;
    }
    pids = malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
    {
        ft_putstr_fd("Memory allocation failed\n", 2);
        return;
    }
    i = 0;
    cmd = shell->cmds;
    while (cmd)
    {
        // Handle heredoc before creating pipe for this command
        heredoc_fd = -1;
        if (cmd->heredoc)
            heredoc_fd = handle_heredoc(shell, cmd);
            
        if (cmd->next && pipe(pipe_fd) == -1)
        {
            ft_putstr_fd("Pipe failed\n", 2);
            shell->exit_status = 1;
            break;
        }
        pids[i] = fork();
        if (pids[i] == -1)
        {
            ft_putstr_fd("Fork failed\n", 2);
            shell->exit_status = 1;
            break;
        }
        if (pids[i] == 0)
        {
            if (prev_fd != STDIN_FILENO)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            
            // Apply heredoc if present (higher priority than prev_fd)
            if (heredoc_fd != -1)
            {
                dup2(heredoc_fd, STDIN_FILENO);
                close(heredoc_fd);
            }
            
            if (cmd->next)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            handle_redirection(cmd, heredoc_fd);
            if (is_builtin(cmd->args[0], builtins))
            {
                status = execute_builtin(shell, cmd, builtins);
                exit(status);
            }
            else
            {
                path = find_path(cmd->args[0], shell->env);
                if (!path || access(path, X_OK) != 0)
                {
                    ft_putstr_fd("Command not found: ", 2);
                    ft_putstr_fd(cmd->args[0], 2);
                    ft_putstr_fd("\n", 2);
                    free(path);
                    exit(127);
                }
                execve(path, cmd->args, env_to_array(shell->env));
                ft_putstr_fd("Execve failed: ", 2);
                ft_putstr_fd(cmd->args[0], 2);
                ft_putstr_fd("\n", 2);
                free(path);
                exit(1);
            }
        }
        // Parent process needs to close heredoc_fd if it was opened
        if (heredoc_fd != -1)
            close(heredoc_fd);
            
        if (prev_fd != STDIN_FILENO)
            close(prev_fd);
        if (cmd->next)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
        cmd = cmd->next;
        i++;
    }
    i = 0;
    while (i < cmd_count)
    {
        waitpid(pids[i], &shell->exit_status, 0);
        if (WIFEXITED(shell->exit_status))
            shell->exit_status = WEXITSTATUS(shell->exit_status);
        i++;
    }
    free(pids);
}

void execute_commands(t_shell *shell, t_builtin *builtins)
{
    t_command   *cmd;
    int         saved_stdin;
    int         saved_stdout;
    int         heredoc_fd;

    cmd = shell->cmds;
    if (!cmd || !cmd->args || !cmd->args[0])
        return;

    heredoc_fd = -1;
    if (cmd->heredoc)
        heredoc_fd = handle_heredoc(shell, cmd);

    if (cmd->next)
    {
        if (heredoc_fd != -1)
            close(heredoc_fd);
        execute_pipeline(shell, builtins);
    }
    else
    {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);

        if (is_builtin(cmd->args[0], builtins))
        {
            // For builtins, we need to handle redirection in the current process
            if (heredoc_fd != -1)
            {
                dup2(heredoc_fd, STDIN_FILENO);
                close(heredoc_fd);
            }
            handle_redirection(cmd, -1); // Don't pass heredoc_fd as it's already set
            shell->exit_status = execute_builtin(shell, cmd, builtins);
        }
        else
        {
            // For external commands
            execute_external(shell, cmd, shell->env, heredoc_fd);
        }

        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
    }
}
