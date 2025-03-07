/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:32:42 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 17:11:00 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_pwd(char **args, t_shell *shell)
{
    (void)args;
    (void)shell;
    char *pwd;

    pwd = getcwd(NULL, 0);
    if (!pwd)
    {
        ft_putstr_fd("minishell: pwd: ", 2);
        ft_putendl_fd(strerror(errno), 2);
        return (1);
    }
    ft_putendl_fd(pwd, 1);
    free(pwd);
    return (0);
}
