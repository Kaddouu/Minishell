/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:41:43 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/06 17:43:27 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, char *src, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (ft_strlen(src));
	while (i < n - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
/*
int	main(int ac, char **av)
{
	if (ac < 1)
		return (0);
	printf("dest AVANT : %s\n", av[1]);
	printf("%zu\n", ft_strlcpy(av[1], av[2], atoi(av[3])));
	printf("dest APRES : %s\n", av[1]);
	//printf("dest AVANT : %s\n", av[1]);
	//printf("%zu\n", strlcpy(av[1], av[2], atoi(av[3])));
	//printf("dest APRES : %s\n", av[1]);
}*/
