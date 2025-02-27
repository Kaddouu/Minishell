/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:26:58 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/12 14:27:19 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*newtab;
	size_t	i;
	size_t	j;

	i = start;
	j = 0;
	if (ft_strlen(s) <= i)
	{
		newtab = malloc(1);
		if (!newtab)
			return (0);
		newtab[0] = '\0';
		return (newtab);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	newtab = malloc(sizeof(char) * len + 1);
	if (!newtab)
		return (0);
	while (j < len && s[i])
		newtab[j++] = s[i++];
	newtab[j] = '\0';
	return (newtab);
}
/*
int	main(int ac, char **av)
{
	if (ac < 1)
		return (0);
	printf("%s\n", ft_substr(av[1], ft_atoi(av[2]), ft_atoi(av[3])));
	return (0);
}*/
