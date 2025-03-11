/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:00:00 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/03/11 12:02:55 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Fonction pour créer un environnement de test
char	**create_test_env(void)
{
	char	**env;

	env = malloc(sizeof(char *) * 4);
	if (!env)
		return (NULL);
	env[0] = ft_strdup("PATH=/usr/bin:/bin");
	env[1] = ft_strdup("HOME=/home/user");
	env[2] = ft_strdup("USER=tester");
	env[3] = NULL;
	return (env);
}

// Fonction pour libérer l'environnement de test
void	free_env_test(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

// Affiche une ligne de séparation
void	print_separator(void)
{
	printf("\n--------------------------------------------------\n");
	fflush(stdout);
}

// Fonction pour tester ft_echo
void	test_echo(void)
{
	char	*args1[] = {"echo", "Hello", "World", NULL};
	char	*args2[] = {"echo", "-n", "Hello", "World", NULL};
	char	*args3[] = {"echo", "-nnn", "Hello", "World", NULL};
	char	*args4[] = {"echo", NULL};

	print_separator();
	printf("TEST DE FT_ECHO\n");
	print_separator();
	printf("Test 1 (echo Hello World) :\n");
	printf("Résultat attendu: Hello World (suivi d'un saut de ligne)\n");
	fflush(stdout);
	ft_echo(args1);
	printf("\nTest 2 (echo -n Hello World) :\n");
	printf("Résultat attendu: Hello World (sans saut de ligne)\n");
	fflush(stdout);
	ft_echo(args2);
	printf("\nTest 3 (echo -nnn Hello World) :\n");
	printf("Résultat attendu: Hello World (sans saut de ligne)\n");
	fflush(stdout);
	ft_echo(args3);
	printf("\nTest 4 (echo) :\n");
	printf("Résultat attendu: (saut de ligne seulement)\n");
	fflush(stdout);
	ft_echo(args4);
}

// Fonction pour tester ft_env
void	test_env(void)
{
	char	**env;

	print_separator();
	printf("TEST DE FT_ENV\n");
	print_separator();
	env = create_test_env();
	if (!env)
	{
		printf("Erreur lors de la création de l'environnement de test\n");
		fflush(stdout);
		return ;
	}
	printf("Test avec un environnement valide :\n");
	printf("Résultat attendu: Liste des variables d'environnement (PATH, HOME, USER)\n");
	fflush(stdout);
	ft_env(env);
	printf("\nTest avec un environnement NULL :\n");
	printf("Résultat attendu: \"Env not found\" (sur stderr)\n");
	fflush(stdout);
	ft_env(NULL);
	free_env_test(env);
}

// Fonction pour tester ft_pwd
void	test_pwd(void)
{
	print_separator();
	printf("TEST DE FT_PWD\n");
	print_separator();
	printf("Répertoire de travail actuel :\n");
	printf("Résultat attendu: Le chemin absolu du répertoire de travail actuel\n");
	fflush(stdout);
	ft_pwd();
}

// Fonction pour tester ft_cd
void	test_cd(void)
{
	char	*cwd_before;
	char	*cwd_after;
	char	*args1[] = {"cd", "..", NULL};
	char	*args2[] = {"cd", "/tmp", NULL};
	char	*args3[] = {"cd", "dossier_inexistant", NULL};
	char	*args4[] = {"cd", NULL};
	int		ret;

	print_separator();
	printf("TEST DE FT_CD\n");
	print_separator();
	// Test 1: cd ..
	cwd_before = getcwd(NULL, 0);
	printf("Test 1: cd ..\n");
	printf("Résultat attendu: Changement au répertoire parent\n");
	printf("Répertoire actuel avant: %s\n", cwd_before);
	fflush(stdout);
	ft_cd(args1);
	cwd_after = getcwd(NULL, 0);
	printf("Répertoire actuel après: %s\n", cwd_after);
	fflush(stdout);
	free(cwd_before);
	free(cwd_after);
	// Test 2: cd /tmp
	cwd_before = getcwd(NULL, 0);
	printf("\nTest 2: cd /tmp\n");
	printf("Résultat attendu: Changement au répertoire /tmp\n");
	printf("Répertoire actuel avant: %s\n", cwd_before);
	fflush(stdout);
	ft_cd(args2);
	cwd_after = getcwd(NULL, 0);
	printf("Répertoire actuel après: %s\n", cwd_after);
	fflush(stdout);
	free(cwd_before);
	free(cwd_after);
	// Test 3: cd vers un dossier inexistant
	printf("\nTest 3: cd vers un dossier inexistant\n");
	printf("Résultat attendu: \"minishell: cd: dossier_inexistant: No such file or directory\"\n");
	fflush(stdout);
	ft_cd(args3);
	// Test 4: cd sans arguments
	printf("\nTest 4: cd sans arguments\n");
	printf("Résultat attendu: Code de retour 1 (aucun changement de répertoire)\n");
	fflush(stdout);
	ret = ft_cd(args4);
	printf("Code de retour: %d\n", ret);
	fflush(stdout);
}

// Fonction pour tester ft_export
void	test_export(void)
{
	char	**env;
	char	*args1[] = {"export", "TEST_VAR=test_value", NULL};
	char	*args2[] = {"export", "123INVALID=value", NULL};
	char	*args3[] = {"export", NULL};

	print_separator();
	printf("TEST DE FT_EXPORT\n");
	print_separator();
	env = create_test_env();
	if (!env)
	{
		printf("Erreur lors de la création de l'environnement de test\n");
		fflush(stdout);
		return ;
	}
	// Afficher l'environnement avant
	printf("Environnement avant export :\n");
	fflush(stdout);
	ft_env(env);
	// Test 1: export une variable valide
	printf("\nTest 1: export d'une variable valide (TEST_VAR=test_value)\n");
	printf("Résultat attendu: Ajout de TEST_VAR=test_value à l'environnement\n");
	fflush(stdout);
	ft_export(args1, &env);
	// Afficher l'environnement après
	printf("Environnement après export :\n");
	fflush(stdout);
	ft_env(env);
	// Test 2: export une variable invalide
	printf("\nTest 2: export d'une variable invalide (123INVALID=value)\n");
	printf("Résultat attendu: \"minishell: export: `123INVALID=value': not a valid identifier\"\n");
	fflush(stdout);
	ft_export(args2, &env);
	// Test 3: export sans arguments
	printf("\nTest 3: export sans arguments\n");
	printf("Résultat attendu: Affichage de toutes les variables d'environnement précédées par \"declare -x\"\n");
	fflush(stdout);
	ft_export(args3, &env);
	free_env_test(env);
}

// Fonction pour tester ft_unset
void	test_unset(void)
{
	char	**env;
	char	*args1[] = {"unset", "USER", NULL};
	char	*args2[] = {"unset", "123INVALID", NULL};
	char	*args3[] = {"unset", "NONEXISTENT", NULL};
	char	*args4[] = {"unset", NULL};
	int		ret;

	print_separator();
	printf("TEST DE FT_UNSET\n");
	print_separator();
	env = create_test_env();
	if (!env)
	{
		printf("Erreur lors de la création de l'environnement de test\n");
		fflush(stdout);
		return ;
	}
	// Afficher l'environnement avant
	printf("Environnement avant unset :\n");
	fflush(stdout);
	ft_env(env);
	// Test 1: unset une variable existante
	printf("\nTest 1: unset d'une variable existante (USER)\n");
	printf("Résultat attendu: Suppression de USER de l'environnement\n");
	fflush(stdout);
	ft_unset(args1, &env);
	// Afficher l'environnement après
	printf("Environnement après unset USER :\n");
	fflush(stdout);
	ft_env(env);
	// Test 2: unset une variable avec un nom invalide
	printf("\nTest 2: unset d'une variable avec un nom invalide (123INVALID)\n");
	printf("Résultat attendu: \"minishell: unset: `123INVALID': not a valid identifier\"\n");
	fflush(stdout);
	ft_unset(args2, &env);
	// Test 3: unset une variable inexistante
	printf("\nTest 3: unset d'une variable inexistante (NONEXISTENT)\n");
	printf("Résultat attendu: Aucune erreur, environnement inchangé\n");
	fflush(stdout);
	ft_unset(args3, &env);
	// Test 4: unset sans arguments
	printf("\nTest 4: unset sans arguments\n");
	printf("Résultat attendu: Aucune action (code de retour 0)\n");
	fflush(stdout);
	ret = ft_unset(args4, &env);
	printf("Code de retour: %d\n", ret);
	fflush(stdout);
	free_env_test(env);
}

// Nous ne testons pas ft_exit car cela terminerait le programme
void	test_exit_info(void)
{
	print_separator();
	printf("TEST DE FT_EXIT (Information seulement)\n");
	print_separator();
	printf("Le test de ft_exit n'est pas effectué car cela terminerait le programme.\n");
	printf("Pour tester ft_exit, vous pouvez utiliser les arguments suivants :\n");
	printf("1. ft_exit([\"exit\", NULL]) - Résultat attendu: Affiche \"exit\" et termine avec code 0\n");
	printf("2. ft_exit([\"exit\", \"42\", NULL]) - Résultat attendu: Affiche \"exit\" et termine avec code 42\n");
	printf("3. ft_exit([\"exit\", \"abc\", NULL]) - Résultat attendu: Affiche un message d'erreur et termine avec code 255\n");
	fflush(stdout);
}

// int	main(int argc, char **argv)
// {
// 	(void)argc;
// 	(void)argv;

// 	printf("=== PROGRAMME DE TEST DES BUILTINS DE MINISHELL ===\n");
// 	fflush(stdout);

// 	test_echo();
// 	test_env();
// 	test_pwd();
// 	test_cd();
// 	test_export();
// 	test_unset();
// 	test_exit_info();

// 	print_separator();
// 	printf("=== TESTS TERMINÉS ===\n");
// 	fflush(stdout);

// 	return (0);
// }