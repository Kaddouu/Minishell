/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:10:52 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/04 15:26:03 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    print_chdir_error(char *path)
{
    ft_putstr_fd("minishell: cd: ", 2);
	ft_putendl_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(path, 2);
}