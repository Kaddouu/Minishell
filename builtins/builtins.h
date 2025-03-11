/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:59:05 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/11 11:36:33 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"
# include "../parsing/parsing.h"

// STRUCTURE POUR CHAQUE BUILTIN
typedef struct s_builtin
{
	char	*name;
	int		(*func)(char **, t_shell *shell);
}			t_builtin;

// Fonctions builtins
int			ft_cd(char **args, t_shell *shell);
int			ft_echo(char **args, t_shell *shell);
int			ft_env(char **envp, t_shell *shell);
int			ft_exit(char **args, t_shell *shell);
int			ft_export(char **args, t_shell *shell);
int			ft_pwd(char **args, t_shell *shell);
int			ft_unset(char **args, t_shell *shell);

#endif
