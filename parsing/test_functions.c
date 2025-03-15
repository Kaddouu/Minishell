/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:34:40 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/14 15:40:52 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void check_token(t_token *token, int expected_type, const char *expected_content)
{
    if (!token || (int)token->type != expected_type || ft_strcmp(token->content, expected_content) != 0)
    {
        printf("Échec: Attendu Type=%d, Contenu='%s', Trouvé Type=%d, Contenu='%s'\n",
               expected_type, expected_content, token ? token->type : -1, token ? token->content : "NULL");
        assert(0); // Arrête le test en cas d'erreur
    }
}

// Fonction utilitaire pour compter les tokens dans une liste
static int count_tokens(t_token *tokens)
{
    int count = 0;
    while (tokens)
    {
        count++;
        tokens = tokens->next;
    }
    return count;
}

void test_lexer(void)
{
    t_shell shell;
	(void)check_token;
	// (void)count_tokens;

    printf("Début des tests du lexer...\n");
    shell.env = NULL; // Initialise l'environnement (ajustez si nécessaire)

    // Test 1 : Commande simple
    {
        char *line = "echo";
        t_token *tokens = lexer(line);
        assert(tokens != NULL && "Lexer a retourné NULL pour 'echo'");
        assert(count_tokens(tokens) == 1 && "Nombre de tokens incorrect");
        assert(tokens->type == WORD && "Type de token incorrect");
        assert(ft_strcmp(tokens->content, "echo") == 0 && "Contenu du token incorrect");
        free_token(tokens);
        printf("Test 1 (Commande simple) : OK\n");
    }

    // Test 2 : Commande avec arguments et espaces multiples
    {
        char *line = "echo   hello   world";
        t_token *tokens = lexer(line);
        assert(tokens != NULL);
        assert(count_tokens(tokens) == 3);
        assert(ft_strcmp(tokens->content, "echo") == 0);
        assert(tokens->type == WORD);
        assert(ft_strcmp(tokens->next->content, "hello") == 0);
        assert(tokens->next->type == WORD);
        assert(ft_strcmp(tokens->next->next->content, "world") == 0);
        assert(tokens->next->next->type == WORD);
        free_token(tokens);
        printf("Test 2 (Arguments avec espaces) : OK\n");
    }

    // Test 3 : Guillemets simples
    {
        char *line = "echo 'hello world'";
        t_token *tokens = lexer(line);
        assert(tokens != NULL && "Lexer a retourné NULL pour 'echo 'hello world''");
        assert(count_tokens(tokens) == 2 && "Nombre de tokens incorrect");
        assert(tokens->type == WORD && "Type du premier token incorrect");
        assert(ft_strcmp(tokens->content, "echo") == 0 && "Contenu du premier token incorrect");
        assert(tokens->next->type == WORD && "Type du deuxième token incorrect");
        assert(ft_strcmp(tokens->next->content, "hello world") == 0 && "Contenu du deuxième token incorrect");
        free_token(tokens);
        printf("Test 3 (Guillemets simples) : OK\n");
    }

    // Test 4 : Redirection complexe
    {
        char *line = "awk '{print $9}' < | >> > | << | file1 | wc -c > file2";
        t_token *tokens = lexer(line);
        assert(tokens != NULL && "Lexer a retourné NULL pour une redirection complexe");
        assert(count_tokens(tokens) == 15 && "Nombre de tokens incorrect");
        t_token *current = tokens;
        assert(ft_strcmp(current->content, "awk") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "{print $9}") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "<") == 0 && current->type == REDIR_IN); current = current->next;
        assert(ft_strcmp(current->content, "|") == 0 && current->type == PIPE); current = current->next;
        assert(ft_strcmp(current->content, ">>") == 0 && current->type == APPEND); current = current->next;
        assert(ft_strcmp(current->content, ">") == 0 && current->type == REDIR_OUT); current = current->next;
        assert(ft_strcmp(current->content, "|") == 0 && current->type == PIPE); current = current->next;
        assert(ft_strcmp(current->content, "<<") == 0 && current->type == HEREDOC); current = current->next;
        assert(ft_strcmp(current->content, "|") == 0 && current->type == PIPE); current = current->next;
        assert(ft_strcmp(current->content, "file1") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "|") == 0 && current->type == PIPE); current = current->next;
        assert(ft_strcmp(current->content, "wc") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "-c") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, ">") == 0 && current->type == REDIR_OUT); current = current->next;
        assert(ft_strcmp(current->content, "file2") == 0 && current->type == WORD);
        free_token(tokens);
        printf("Test 4 (Redirection complexe) : OK\n");
    }

    // Test 5 : Pipes multiples
    {
        char *line = "echo hello | grep e | wc -l";
        t_token *tokens = lexer(line);
        assert(tokens != NULL && "Lexer a retourné NULL pour des pipes multiples");
        assert(count_tokens(tokens) == 8 && "Nombre de tokens incorrect");
        t_token *current = tokens;
        assert(ft_strcmp(current->content, "echo") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "hello") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "|") == 0 && current->type == PIPE); current = current->next;
        assert(ft_strcmp(current->content, "grep") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "e") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "|") == 0 && current->type == PIPE); current = current->next;
        assert(ft_strcmp(current->content, "wc") == 0 && current->type == WORD); current = current->next;
        assert(ft_strcmp(current->content, "-l") == 0 && current->type == WORD);
        free_token(tokens);
        printf("Test 5 (Pipes multiples) : OK\n");
    }

    // Test 6 : Variables d'environnement
    {
        char *line = "echo $HOME $PATH";
        t_token *tokens = lexer(line);
        assert(tokens != NULL && "Lexer a retourné NULL pour des variables d'environnement");
        assert(count_tokens(tokens) == 3 && "Nombre de tokens incorrect");
        assert(ft_strcmp(tokens->content, "echo") == 0 && tokens->type == WORD);
        assert(ft_strcmp(tokens->next->content, "HOME") == 0 && tokens->next->type == ENV);
        assert(ft_strcmp(tokens->next->next->content, "PATH") == 0 && tokens->next->next->type == ENV);
        free_token(tokens);
        printf("Test 6 (Variables d'environnement) : OK\n");	
    }

	// (void)count_tokens;
    // Test 7 : Ligne vide
    {
        char *line = "   ";
        t_token *tokens = lexer(line);
        assert(tokens == NULL && "Tokens inattendus pour une ligne vide");
        free_token(tokens);
        printf("Test 7 (Ligne vide) : OK\n");
    }

    printf("Tous les tests du lexer ont réussi !\n");
}

// Test de la fonction parser
void test_parser(void)
{
    printf("Début des tests de parser\n");

    // Test 1: Commande simple
    {
        t_token *tokens = create_token(ft_strdup("echo"), WORD);
        t_command *cmds = parser(tokens);
        assert(cmds != NULL);
        assert(ft_strcmp(cmds->args[0], "echo") == 0);
        assert(cmds->args[1] == NULL);
        free_token(tokens);
        free_command(cmds);
    }

    // Test 2: Commande avec arguments
    {
        t_token *tokens = create_token(ft_strdup("echo"), WORD);
        t_token *last = tokens;
        add_token(&tokens, &last, create_token(ft_strdup("hello"), WORD));
        add_token(&tokens, &last, create_token(ft_strdup("world"), WORD));
        t_command *cmds = parser(tokens);
        assert(cmds != NULL);
        assert(ft_strcmp(cmds->args[0], "echo") == 0);
        assert(ft_strcmp(cmds->args[1], "hello") == 0);
        assert(ft_strcmp(cmds->args[2], "world") == 0);
        assert(cmds->args[3] == NULL);
        free_token(tokens);
        free_command(cmds);
    }

	{
	    t_token *tokens = create_token(ft_strdup("echo"), WORD);
	    t_token *last = tokens;
	    add_token(&tokens, &last, create_token(ft_strdup("hello"), WORD));
	    add_token(&tokens, &last, create_token(ft_strdup(">"), REDIR_OUT));
	    add_token(&tokens, &last, create_token(ft_strdup("output.txt"), WORD));
	    t_command *cmds = parser(tokens);
	    assert(cmds != NULL);
	    assert(ft_strcmp(cmds->args[0], "echo") == 0);
	    assert(ft_strcmp(cmds->args[1], "hello") == 0);
	    assert(cmds->args[2] == NULL);
	    assert(ft_strcmp(cmds->output, "output.txt") == 0);
	    free_token(tokens);
	    free_command(cmds);
	}
	
	{
	    t_token *tokens = create_token(ft_strdup("cat"), WORD);
	    t_token *last = tokens;
	    add_token(&tokens, &last, create_token(ft_strdup("<"), REDIR_IN));
	    add_token(&tokens, &last, create_token(ft_strdup("input.txt"), WORD));
	    t_command *cmds = parser(tokens);
	    assert(cmds != NULL);
	    assert(ft_strcmp(cmds->args[0], "cat") == 0);
	    assert(cmds->args[1] == NULL);
	    assert(ft_strcmp(cmds->input, "input.txt") == 0);
	    free_token(tokens);
	    free_command(cmds);
	}
	
	{
	    t_token *tokens = create_token(ft_strdup("echo"), WORD);
	    t_token *last = tokens;
	    add_token(&tokens, &last, create_token(ft_strdup("hello"), WORD));
	    add_token(&tokens, &last, create_token(ft_strdup("|"), PIPE));
	    add_token(&tokens, &last, create_token(ft_strdup("wc"), WORD));
	    add_token(&tokens, &last, create_token(ft_strdup("-l"), WORD));
	    t_command *cmds = parser(tokens);
	    assert(cmds != NULL);
	    assert(ft_strcmp(cmds->args[0], "echo") == 0);
	    assert(ft_strcmp(cmds->args[1], "hello") == 0);
	    assert(cmds->args[2] == NULL);
	    assert(cmds->next != NULL);
	    assert(ft_strcmp(cmds->next->args[0], "wc") == 0);
	    assert(ft_strcmp(cmds->next->args[1], "-l") == 0);
	    assert(cmds->next->args[2] == NULL);
	    free_token(tokens);
	    free_command(cmds);
	}
	
	{
	    t_token *tokens = create_token(ft_strdup("echo"), WORD);
	    t_token *last = tokens;
	    add_token(&tokens, &last, create_token(ft_strdup("hello"), WORD));
	    add_token(&tokens, &last, create_token(ft_strdup("|"), PIPE));
	    add_token(&tokens, &last, create_token(ft_strdup("grep"), WORD));
	    add_token(&tokens, &last, create_token(ft_strdup("e"), WORD));
	    add_token(&tokens, &last, create_token(ft_strdup("|"), PIPE));
	    add_token(&tokens, &last, create_token(ft_strdup("wc"), WORD));
	    add_token(&tokens, &last, create_token(ft_strdup("-l"), WORD));
	    t_command *cmds = parser(tokens);
	    assert(cmds != NULL);
	    assert(ft_strcmp(cmds->args[0], "echo") == 0);
	    assert(ft_strcmp(cmds->args[1], "hello") == 0);
	    assert(cmds->next != NULL);
	    assert(ft_strcmp(cmds->next->args[0], "grep") == 0);
	    assert(ft_strcmp(cmds->next->args[1], "e") == 0);
	    assert(cmds->next->next != NULL);
	    assert(ft_strcmp(cmds->next->next->args[0], "wc") == 0);
	    assert(ft_strcmp(cmds->next->next->args[1], "-l") == 0);
	    free_token(tokens);
	    free_command(cmds);
	}
	
	{
	    t_token *tokens = create_token(ft_strdup("echo"), WORD);
	    t_token *last = tokens;
	    add_token(&tokens, &last, create_token(ft_strdup("hello world"), WORD)); // Après gestion des guillemets par le lexer
	    t_command *cmds = parser(tokens);
	    assert(cmds != NULL);
	    assert(ft_strcmp(cmds->args[0], "echo") == 0);
	    assert(ft_strcmp(cmds->args[1], "hello world") == 0);
	    assert(cmds->args[2] == NULL);
	    free_token(tokens);
	    free_command(cmds);
	}
	
	{
	    t_token *tokens = create_token(ft_strdup("cat"), WORD);
	    t_token *last = tokens;
	    add_token(&tokens, &last, create_token(ft_strdup("<<"), HEREDOC));
	    add_token(&tokens, &last, create_token(ft_strdup("EOF"), WORD));
	    t_command *cmds = parser(tokens);
	    assert(cmds != NULL);
	    assert(ft_strcmp(cmds->args[0], "cat") == 0);
	    assert(cmds->args[1] == NULL);
	    assert(ft_strcmp(cmds->heredoc, "EOF") == 0);
	    free_token(tokens);
	    free_command(cmds);
	}
	
	{
	    t_token *tokens = create_token(ft_strdup("echo"), WORD);
	    t_token *last = tokens;
	    add_token(&tokens, &last, create_token(ft_strdup("|"), PIPE));
	    add_token(&tokens, &last, create_token(ft_strdup("|"), PIPE));
	    add_token(&tokens, &last, create_token(ft_strdup("wc"), WORD));
	    t_command *cmds = parser(tokens);
	    assert(cmds == NULL); // Ou autre gestion d'erreur selon votre implémentation
	    free_token(tokens);
	    free_command(cmds);
	}

    printf("Tous les tests de parser ont réussi !\n");
}

// Test de la fonction find_env_var
void	test_find_env_var(void)
{
	t_env	*env;
	t_env	*result;

	env = malloc(sizeof(t_env));
	env->key = ft_strdup("HOME");
	env->value = ft_strdup("/home/user");
	env->next = NULL;
	result = find_env_var(env, "HOME");
	if (result != NULL)
		printf("Variable trouvée: %s = %s\n", result->key, result->value);
	else
		printf("Variable non trouvée.\n");
	// Test pour une variable non existante
	result = find_env_var(env, "PATH");
	if (result == NULL)
		printf("Variable PATH non trouvée, comme prévu.\n");
	// Libération de la mémoire
	free(env->key);
	free(env->value);
	free(env);
}

// Test de la fonction expand_all_env_vars
void	test_expand_all_env_vars(void)
{
	t_env	*env;
	t_token	*tokens;

	env = malloc(sizeof(t_env));
	env->key = ft_strdup("HOME");
	env->value = ft_strdup("/home/user");
	env->next = NULL;
	tokens = create_token(ft_strdup("$HOME"), ENV);
	expand_all_env_vars(tokens, env, 1);
	// Vérifiez que la variable a été étendue
	if (strcmp(tokens->content, "/home/user") == 0)
		printf("Variable étendue: %s\n", tokens->content);
	else
		printf("Erreur d'expansion de la variable.\n");
	// Libération de la mémoire
	free(env->key);
	free(env->value);
	free(env);
	free_token(tokens);
}

// Test de la fonction expand_variables
void	test_expand_variables(void)
{
	t_env	*env;
	char	*result;

	env = malloc(sizeof(t_env));
	env->key = ft_strdup("USER");
	env->value = ft_strdup("testuser");
	env->next = NULL;
	result = expand_variables("Hello $USER!", env);
	printf("Résultat de l'expansion: %s\n", result);
	// Libération de la mémoire
	free(env->key);
	free(env->value);
	free(env);
	free(result);
}

// Test de la fonction handle_input
void	test_handle_input(void)
{
	t_shell	shell;
	char	*line;
	int		result;

	shell.env = NULL;
	// Remplissez avec des variables d'environnement si nécessaire
	line = "echo $USER";
	result = handle_input(&shell, line);
	if (result == 0)
		printf("Entrée traitée avec succès.\n");
	else
		printf("Erreur lors du traitement de l'entrée.\n");
}

// Test de la fonction create_command
void	test_create_command(void)
{
	t_command	*cmd;

	cmd = create_command();
	assert(cmd == NULL);
	assert(cmd->args == NULL);
	assert(cmd->args[0] == NULL);
	// Vérifiez que le tableau d'arguments est initialisé à NULL
	// Libération de la mémoire
	free_command(cmd);
}

// Test de la fonction add_command
void	test_add_command(void)
{
	t_command	*cmds;
	t_command	*cmd1;
	t_command	*cmd2;

	cmds = NULL;
	cmd1 = create_command();
	cmd2 = create_command();
	add_command(&cmds, cmd1);
	assert(cmds == cmd1); // cmd1 doit être le premier élément
	add_command(&cmds, cmd2);
	assert(cmds->next == cmd2); // cmd2 doit être le deuxième élément
	// Libération de la mémoire
	free_command(cmd1);
	free_command(cmd2);
	free_command(cmds);
}

// Test de la fonction add_arg_to_command
void	test_add_arg_to_command(void)
{
	t_command	*cmd;

	cmd = create_command();
	add_arg_to_command(cmd, "arg1");
	assert(strcmp(cmd->args[0], "arg1") == 0);
	assert(cmd->args[1] == NULL);
	// Vérifiez que le tableau est correctement terminé
	add_arg_to_command(cmd, "arg2");
	assert(strcmp(cmd->args[1], "arg2") == 0);
	// Libération de la mémoire
	free_command(cmd);
}

// Test de la fonction create_token
void	test_create_token(void)
{
	t_token	*token;

	token = create_token(ft_strdup("test"), WORD);
	assert(token != NULL);
	assert(strcmp(token->content, "test") == 0);
	assert(token->type == WORD);
	// Libération de la mémoire
	free(token->content);
	free(token);
}

// Test de la fonction add_token
void	test_add_token(void)
{
	t_token	*tokens;
	t_token	*last;
	t_token	*token1;
	t_token	*token2;

	tokens = NULL;
	last = NULL;
	token1 = create_token(ft_strdup("token1"), WORD);
	add_token(&tokens, &last, token1);
	assert(tokens == token1);
	assert(last == token1);
	token2 = create_token(ft_strdup("token2"), WORD);
	add_token(&tokens, &last, token2);
	assert(last == token2);
	assert(tokens->next == token2);
	// Libération de la mémoire
	free_token(tokens);
}

// Test de la fonction handle_word
void	test_handle_word(void)
{
	char	*input;
	char	*ptr;
	t_token	*tokens;
	t_token	*last;

	input = "hello world";
	ptr = input;
	tokens = NULL;
	last = NULL;
	handle_word(&tokens, &last, &ptr);
	assert(strcmp(tokens->content, "hello") == 0);
	assert(tokens->next == NULL);
	// Libération de la mémoire
	free_token(tokens);
}

// Test de la fonction ft_strjoin_free
void	test_ft_strjoin_free(void)
{
	char	*result;

	result = ft_strjoin_free(ft_strdup("Hello, "), ft_strdup("World!"));
	assert(strcmp(result, "Hello, World!") == 0);
	free(result); // Libération de la mémoire
}

// Test de la fonction ft_strjoin_char_free
void	test_ft_strjoin_char_free(void)
{
	char	*result;

	result = ft_strjoin_char_free(ft_strdup("Hello"), '!');
	assert(strcmp(result, "Hello!") == 0);
	free(result); // Libération de la mémoire
}

// Test de la fonction execute_commands
// void	test_execute_commands(void)
// {
// 	t_shell	shell;

// 	shell.tokens = NULL;      // Remplissez avec des tokens si nécessaire
// 	execute_commands(&shell); // Vérifiez que cela ne plante pas
// }

// int	main(void)
// {
	// test_lexer();
// 	test_parser();
// 	test_find_env_var();
// 	test_expand_all_env_vars();
// 	test_expand_variables();
// 	test_handle_input();
// 	// test_create_command();
// 	// test_add_command();
// 	// test_add_arg_to_command();
// 	// test_create_token();
// 	// test_add_token();
// 	// test_handle_word();
// 	// test_ft_strjoin_free();
// 	// test_ft_strjoin_char_free();
// 	printf("Tous les tests ont réussi !\n");
// 	return (0);
// }
