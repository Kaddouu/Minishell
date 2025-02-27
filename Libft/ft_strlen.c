/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:35:40 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/06 17:38:25 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
/*
int	main(int ac, char **av)
{
	if (ac > 2)
	{
		printf("Il y a trop de chaine !! Veuillez reessayer.\n");
		return (0);
	}
	printf("\nla taille de ma chaine est : { %zu }\n\n", ft_strlen(av[1]));
	printf("la taille de ma chaine est : { %zu }\n\n", strlen(av[1]));
}*/
