/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:53:47 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/12 15:58:22 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newtab;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	newtab = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!newtab)
		return (0);
	while (s1[i])
		newtab[j++] = s1[i++];
	i = 0;
	while (s2[i])
		newtab[j++] = s2[i++];
	newtab[j] = '\0';
	return (newtab);
}
/*
int	main(int ac, char **av)
{
	if (ac < 1)
		return (0);
	printf("%s\n", ft_strjoin(av[1], av[2]));
}*/
