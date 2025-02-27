/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:20:13 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/14 16:20:35 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}
/*
int	main(int ac, char **av)
{
	if (ac < 1)
		return (0);
	ft_putendl_fd(av[1], ft_atoi(av[3]));
	ft_putendl_fd(av[2], ft_atoi(av[3]));
}*/
