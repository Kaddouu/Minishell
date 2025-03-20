/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/20 13:24:36 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_current_cmd(t_command **cmds, t_command **current_cmd)
{
	*current_cmd = create_command();
	if (!*current_cmd)
	{
		free_command(*cmds);
		return (0);
	}
	add_command(cmds, *current_cmd);
	return (1);
}

static int	validate_pipe(t_command *current_cmd, t_command **cmds)
{
	if (!current_cmd->args || (!current_cmd->args[0] && !current_cmd->input
			&& !current_cmd->output && !current_cmd->heredoc
			&& !current_cmd->append))
	{
		printf(" syntax error near unexpected token `|'\n");
		free_command(*cmds);
		return (0);
	}
	return (1);
}

static char	**get_redirection_target(t_command *cmd, t_type type)
{
	if (type == REDIR_IN)
		return (&cmd->input);
	else if (type == REDIR_OUT)
		return (&cmd->output);
	else if (type == HEREDOC)
		return (&cmd->heredoc);
	else
		return (&cmd->append);
}

static int	check_redirection_file(t_parser_context *ctx, t_type type)
{
	const char	*err_msg[] = {"Erreur : redirection d'entrée sans fichier",
			"Erreur : redirection de sortie sans fichier",
			"Erreur : heredoc sans délimiteur",
			"Erreur : redirection en append sans fichier"};

	*ctx->current = (*ctx->current)->next;
	if (!*ctx->current || (*ctx->current)->type != WORD)
	{
		printf("%s\n", err_msg[type - REDIR_IN]);
		free_command(*ctx->cmds);
		return (0);
	}
	return (1);
}

int	redirection(t_parser_context *ctx, t_type type)
{
	char		**target;
	t_command	*cmd;

	cmd = *ctx->current_cmd;
	if (!check_redirection_file(ctx, type))
		return (0);
	target = get_redirection_target(cmd, type);
	if (*target)
		free(*target);
	*target = ft_strdup((*ctx->current)->content);
	if (!*target)
	{
		free_command(*ctx->cmds);
		return (0);
	}
	*ctx->current = (*ctx->current)->next;
	return (1);
}

static int	handle_quoted_delimiter(char **delimiter, t_command *cmd,
		t_command **cmds)
{
	char	quote;
	size_t	len;
	char	*new_delimiter;

	quote = (*delimiter)[0];
	len = ft_strlen(*delimiter);
	if (len > 1 && (*delimiter)[len - 1] == quote)
	{
		new_delimiter = ft_substr(*delimiter, 1, len - 2);
		free(*delimiter);
		if (!new_delimiter)
			return (free_command(*cmds), 0);
		*delimiter = new_delimiter;
		cmd->expand_heredoc = 0;
		return (1);
	}
	else
	{
		printf("Erreur : guillemet non fermé dans le délimiteur du heredoc\n");
		free(*delimiter);
		free_command(*cmds);
		return (0);
	}
}

static int	check_heredoc_delimiter(t_parser_context *ctx)
{
	*ctx->current = (*ctx->current)->next;
	if (!*ctx->current || (*ctx->current)->type != WORD)
	{
		printf("Erreur : heredoc sans délimiteur\n");
		free_command(*ctx->cmds);
		return (0);
	}
	return (1);
}

int	process_heredoc_delimiter(t_parser_context *ctx)
{
	char		*delimiter;
	t_command	*cmd;

	cmd = *ctx->current_cmd;
	if (!check_heredoc_delimiter(ctx))
		return (0);
	if (cmd->heredoc)
		free(cmd->heredoc);
	delimiter = ft_strdup((*ctx->current)->content);
	if (!delimiter)
	{
		free_command(*ctx->cmds);
		return (0);
	}
	if (delimiter[0] == '\'' || delimiter[0] == '"')
	{
		if (!handle_quoted_delimiter(&delimiter, cmd, ctx->cmds))
			return (0);
	}
	else
		cmd->expand_heredoc = 1;
	cmd->heredoc = delimiter;
	*ctx->current = (*ctx->current)->next;
	return (1);
}

int	handle_pipe_sequence(t_parser_context *ctx)
{
	if (!validate_pipe(*ctx->current_cmd, ctx->cmds))
		return (0);
	*ctx->current = (*ctx->current)->next;
	if (!*ctx->current || (*ctx->current)->type == PIPE)
	{
		printf("Erreur : pipe suivi d'un pipe ou fin de ligne\n");
		free_command(*ctx->cmds);
		return (0);
	}
	if (!init_current_cmd(ctx->cmds, ctx->current_cmd))
		return (0);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/21 11:32:27 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	process_token_word_env(t_parser_context *ctx)
{
	if ((*ctx->current)->type == WORD || (*ctx->current)->type == ENV)
	{
		add_arg_to_command(*ctx->current_cmd, (*ctx->current)->content);
		*ctx->current = (*ctx->current)->next;
		return (1);
	}
	else
	{
		printf("Erreur : type de token inattendu\n");
		free_command(*ctx->cmds);
		return (0);
	}
}

int	process_token(t_parser_context *ctx)
{
	if ((*ctx->current)->type == PIPE)
	{
		if (!handle_pipe_sequence(ctx))
			return (0);
	}
	else if ((*ctx->current)->type == REDIR_IN
		|| (*ctx->current)->type == REDIR_OUT
		|| (*ctx->current)->type == APPEND)
	{
		if (!redirection(ctx, (*ctx->current)->type))
			return (0);
	}
	else if ((*ctx->current)->type == HEREDOC)
	{
		if (!process_heredoc_delimiter(ctx))
			return (0);
	}
	else if (!process_token_word_env(ctx))
		return (0);
	return (1);
}

t_command	*parser(t_token *tokens)
{
	t_command			*cmds;
	t_command			*current_cmd;
	t_token				*current;
	t_parser_context	ctx;

	cmds = NULL;
	current_cmd = NULL;
	current = tokens;
	if (!tokens)
		return (NULL);
	ctx.cmds = &cmds;
	ctx.current_cmd = &current_cmd;
	ctx.current = &current;
	while (current)
	{
		if (!current_cmd && !init_current_cmd(&cmds, &current_cmd))
			return (NULL);
		if (!process_token(&ctx))
			return (NULL);
	}
	return (cmds);
}
