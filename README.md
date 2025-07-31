# Minishell

Minishell est un projet réalisé dans le cadre du cursus 42. Il s'agit d'une implémentation simplifiée d'un shell Unix, permettant d'exécuter des commandes, de gérer les redirections, les pipes, les variables d'environnement, et d'implémenter plusieurs builtins.

## Objectifs du projet
- Comprendre le fonctionnement d'un shell Unix.
- Manipuler les processus, pipes, signaux et gestion de la mémoire en C.
- Implémenter les commandes internes (builtins) : `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`.
- Gérer les redirections (`>`, `>>`, `<`, `<<`) et les pipes (`|`).
- Gérer les variables d'environnement et leur expansion.
- Gérer les signaux (Ctrl+C, Ctrl+\, etc).

## Fonctionnalités principales
- Exécution de commandes simples et enchaînées par des pipes.
- Gestion des redirections d'entrée et de sortie.
- Implémentation des builtins.
- Gestion des erreurs et des signaux.
- Support du heredoc.

## Structure du projet
- `minishell.c` : Point d'entrée du programme.
- `builtins/` : Implémentation des commandes internes.
- `exec/` : Gestion de l'exécution des commandes, pipes, redirections.
- `parsing/` : Analyse et traitement de la ligne de commande.
- `Libft/` : Fonctions utilitaires (libft).

## Compilation

Utilisez la commande suivante pour compiler le projet :

```sh
make
```

## Utilisation

Lancez le shell avec :

```sh
./minishell
```

Vous pouvez alors entrer des commandes comme dans un shell classique.
