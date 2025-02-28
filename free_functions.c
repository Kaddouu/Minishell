/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:37:19 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/28 15:37:55 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->content);
		free(token);
		token = tmp;
	}
}

void	free_command_content(t_command *cmd)
{
	int	i;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	free(cmd->input);
	free(cmd->output);
	free(cmd->heredoc);
	free(cmd->append);
}

void	free_command(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_command_content(cmd);
		free(cmd);
		cmd = tmp;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_env(shell->env);
	free_token(shell->tokens);
	free_command(shell->cmds);
	free(shell->last_cmd);
	free(shell);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_strjoin_char_free(char *s, char c)
{
	char	*result;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	result = (char *)malloc(len + 2);
	if (!result)
	{
		free(s);
		return (NULL);
	}
	ft_strcpy(result, s);
	result[len] = c;
	result[len + 1] = '\0';
	free(s);
	return (result);
}
