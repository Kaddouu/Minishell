/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:40:01 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/11 11:43:33 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input(t_shell *shell, char *line)
{
	if (!line || !shell)
		return (-1);
	if (ft_strcmp(line, "exit") == 0)
		return (-1);
	shell->tokens = lexer(line);
	if (!shell->tokens)
		return (0);
	expand_all_env_vars(shell->tokens, shell->env);
	shell->cmds = parser(shell->tokens);
	if (!shell->cmds)
	{
		free_token(shell->tokens);
		shell->tokens = NULL;
		return (0);
	}
	free_token(shell->tokens);
	shell->tokens = NULL;
	free_command(shell->cmds);
	shell->cmds = NULL;
	return (0);
}

t_command	*parser(t_token *tokens)
{
	t_command	*cmds;
	t_command	*current_cmd;
	t_token		*current;
	char		*delimiter;
	char		quote;
	size_t		len;
	char		*new_delimiter;

	cmds = NULL;
	current_cmd = NULL;
	current = tokens;
	if (!tokens)
		return (NULL);
	while (current)
	{
		if (!current_cmd)
		{
			current_cmd = create_command();
			if (!current_cmd)
			{
				free_command(cmds);
				return (NULL);
			}
			add_command(&cmds, current_cmd);
		}
		if (current->type == PIPE)
		{
			if (!current_cmd->args || (!current_cmd->args[0]
					&& !current_cmd->input && !current_cmd->output
					&& !current_cmd->heredoc && !current_cmd->append))
			{
				printf("Erreur : pipe sans commande\n");
				free_command(cmds);
				return (NULL);
			}
			current = current->next;
			if (!current || current->type == PIPE)
			{
				printf("Erreur : pipe suivi d'un pipe ou fin de ligne\n");
				free_command(cmds);
				return (NULL);
			}
			current_cmd = create_command();
			if (!current_cmd)
			{
				free_command(cmds);
				return (NULL);
			}
			add_command(&cmds, current_cmd);
		}
		else if (current->type == REDIR_IN)
		{
			current = current->next;
			if (!current || current->type != WORD)
			{
				printf("Erreur : redirection d'entrée sans fichier\n");
				free_command(cmds);
				return (NULL);
			}
			current_cmd->input = ft_strdup(current->content);
			if (!current_cmd->input)
			{
				free_command(cmds);
				return (NULL);
			}
			current = current->next;
		}
		else if (current->type == REDIR_OUT)
		{
			current = current->next;
			if (!current || current->type != WORD)
			{
				printf("Erreur : redirection de sortie sans fichier\n");
				free_command(cmds);
				return (NULL);
			}
			current_cmd->output = ft_strdup(current->content);
			if (!current_cmd->output)
			{
				free_command(cmds);
				return (NULL);
			}
			current = current->next;
		}
		else if (current->type == HEREDOC)
		{
			current = current->next;
			if (!current || current->type != WORD)
			{
				printf("Erreur : heredoc sans délimiteur\n");
				free_command(cmds);
				return (NULL);
			}
			delimiter = ft_strdup(current->content);
			if (!delimiter)
			{
				free_command(cmds);
				return (NULL);
			}
			if (delimiter[0] == '\'' || delimiter[0] == '"')
			{
				quote = delimiter[0];
				len = ft_strlen(delimiter);
				if (len > 1 && delimiter[len - 1] == quote)
				{
					new_delimiter = ft_substr(delimiter, 1, len - 2);
					free(delimiter);
					if (!new_delimiter)
					{
						free_command(cmds);
						return (NULL);
					}
					delimiter = new_delimiter;
					current_cmd->expand_heredoc = 0;
				}
				else
				{
					printf("Erreur : guillemet non fermé dans le délimiteur du heredoc\n");
					free(delimiter);
					free_command(cmds);
					return (NULL);
				}
			}
			else
			{
				current_cmd->expand_heredoc = 1;
			}
			current_cmd->heredoc = delimiter;
			current = current->next;
		}
		else if (current->type == APPEND)
		{
			current = current->next;
			if (!current || current->type != WORD)
			{
				printf("Erreur : redirection en append sans fichier\n");
				free_command(cmds);
				return (NULL);
			}
			current_cmd->append = ft_strdup(current->content);
			if (!current_cmd->append)
			{
				free_command(cmds);
				return (NULL);
			}
			current = current->next;
		}
		else if (current->type == WORD || current->type == ENV)
		{
			add_arg_to_command(current_cmd, current->content);
			current = current->next;
		}
		else
		{
			printf("Erreur : type de token inattendu\n");
			free_command(cmds);
			return (NULL);
		}
	}
	return cmds;
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	t_token	*last;
	char	*ptr;

	tokens = NULL;
	last = NULL;
	ptr = input;
	while (*ptr)
	{
		if (ft_isspace(*ptr))
		{
			ptr++;
			continue ;
		}
		else if (*ptr == '|')
		{
			add_token(&tokens, &last, create_token(ft_strdup("|"), PIPE));
			ptr++;
		}
		else if (*ptr == '<' && *(ptr + 1) == '<')
		{
			add_token(&tokens, &last, create_token(ft_strdup("<<"), HEREDOC));
			ptr += 2;
		}
		else if (*ptr == '>' && *(ptr + 1) == '>')
		{
			add_token(&tokens, &last, create_token(ft_strdup(">>"), APPEND));
			ptr += 2;
		}
		else if (*ptr == '<')
		{
			add_token(&tokens, &last, create_token(ft_strdup("<"), REDIR_IN));
			ptr++;
		}
		else if (*ptr == '>')
		{
			add_token(&tokens, &last, create_token(ft_strdup(">"), REDIR_OUT));
			ptr++;
		}
		else if (*ptr == '$')
			handle_env_var(&tokens, &last, &ptr);
		else if (*ptr == '\'' || *ptr == '"')
		{
			if (!handle_quotes(&tokens, &last, &ptr))
			{
				free_token(tokens);
				return NULL;
			}
		}
		else
			handle_word(&tokens, &last, &ptr);
	}
	return tokens;
}

t_token	*create_token(char *content, t_type type)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = content;
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **tokens, t_token **last, t_token *new_token)
{
	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		*last = new_token;
	}
	else
	{
		(*last)->next = new_token;
		*last = new_token;
	}
}

void	handle_env_var(t_token **tokens, t_token **last, char **ptr)
{
	char	*start;
	char	*var_name;

	(*ptr)++;
	start = *ptr;
	while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
		(*ptr)++;
	var_name = ft_substr(start, 0, *ptr - start);
	add_token(tokens, last, create_token(var_name, ENV));
}

void	handle_word(t_token **tokens, t_token **last, char **ptr)
{
	char	*start;
	char	*word;

	start = *ptr;
	while (**ptr && !ft_isspace(**ptr) && **ptr != '|' && **ptr != '<'
		&& **ptr != '>' && **ptr != '\'' && **ptr != '"' && **ptr != '$')
		(*ptr)++;
	word = ft_substr(start, 0, *ptr - start);
	add_token(tokens, last, create_token(word, WORD));
}

int	handle_quotes(t_token **tokens, t_token **last, char **ptr)
{
	char	quote;
	char	*start;
	char	*content;
	t_token	*new_token;

	if (!tokens || !last || !ptr || !*ptr)
		return 0;
	quote = **ptr;
	(*ptr)++;
	start = *ptr;
	while (**ptr && **ptr != quote)
	{
		if (quote == '"' && **ptr == '$')
		{
			handle_env_var(tokens, last, ptr);
			start = *ptr;
		}
		else
			(*ptr)++;
	}
	if (!**ptr)
	{
		printf("Erreur : guillemet non fermé\n");
		return 0;
	}
	content = ft_substr(start, 0, *ptr - start);
	if (!content)
		return 0;
	new_token = create_token(content, WORD);
	if (!new_token)
	{
		free(content);
		return 0;
	}
	add_token(tokens, last, new_token);
	(*ptr)++;
	return 1;
}

t_env	*find_env_var(t_env *env, char *var_name)
{
	while (env)
	{
		if (ft_strcmp(env->key, var_name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	expand_all_env_vars(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded;
	t_env	*var;

	current = tokens;
	while (current)
	{
		if (current->type == ENV)
		{
			var = find_env_var(env, current->content);
			if (var)
			{
				free(current->content);
				current->content = ft_strdup(var->value);
				current->type = WORD;
			}
			else
			{
				free(current->content);
				current->content = ft_strdup("");
				current->type = WORD;
			}
		}
		else if (current->type == WORD)
		{
			if (ft_strchr(current->content, '$'))
			{
				expanded = expand_variables(current->content, env);
				free(current->content);
				current->content = expanded;
			}
		}
		current = current->next;
	}
}

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->heredoc = NULL;
	cmd->expand_heredoc = 1;
	cmd->append = NULL;
	cmd->next = NULL;
	return cmd;
}

void	add_command(t_command **cmds, t_command *new_cmd)
{
	t_command	*current;

	if (!new_cmd)
		return ;
	if (!*cmds)
		*cmds = new_cmd;
	else
	{
		current = *cmds;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
}

void	add_arg_to_command(t_command *cmd, char *arg)
{
	int		i;
	char	**new_args;

	if (!cmd || !arg)
		return ;
	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new_args = realloc(cmd->args, sizeof(char *) * (i + 2));
	if (!new_args)
	{
		free_command_content(cmd);
		return ;
	}
	cmd->args = new_args;
	cmd->args[i] = ft_strdup(arg);
	if (!cmd->args[i])
	{
		free_command_content(cmd);
		return ;
	}
	cmd->args[i + 1] = NULL;
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	*ptr;
	char	*start;
	char	*var_name;
	t_env	*env_var;
	char	*temp;
	char	*temp;

	if (!str || !env)
		return NULL;
	result = ft_strdup("");
	if (!result)
		return NULL;
	ptr = str;
	while (*ptr)
	{
		if (*ptr == '$' && *(ptr + 1) && (ft_isalnum(*(ptr + 1)) || *(ptr
					+ 1) == '_'))
		{
			ptr++;
			start = ptr;
			while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
				ptr++;
			var_name = ft_substr(start, 0, ptr - start);
			if (!var_name)
			{
				free(result);
				return NULL;
			}
			env_var = find_env_var(env, var_name);
			if (env_var)
			{
				temp = ft_strjoin_free(result, env_var->value);
				if (!temp)
				{
					free(var_name);
					free(result);
					return NULL;
				}
				result = temp;
			}
			free(var_name);
		}
		else
		{
			temp = ft_strjoin_char_free(result, *ptr);
			if (!temp)
			{
				free(result);
				return NULL;
			}
			result = temp;
			ptr++;
		}
	}
	return result;
}
