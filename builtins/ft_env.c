/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:50:04 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/04 14:15:18 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(char **env)
{
	int	i;

	i = 0;
    if (env)
    {
        while (env[i])
        {
            ft_putendl_fd(env[i], 1);
            i++;
        }
        return (0);       
    }
    ft_putendl_fd("Env not found", 2);
    return (1);
}