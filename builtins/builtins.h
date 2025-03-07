/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:59:05 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 14:52:08 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"
#include <limits.h>

int		ft_echo(char **args);
int		ft_cd(char **args);
int		ft_pwd(void);
int		ft_export(char **args, char ***envp);
int		ft_unset(char **args, char ***envp);
int		ft_env(char **envp);
int		ft_exit(char **args);

#endif
