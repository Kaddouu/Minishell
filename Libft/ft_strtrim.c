/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 16:28:48 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/12 16:29:17 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	in_set(char const *set, char c)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (0);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && in_set(set, s1[start]))
		start++;
	while (start < end && in_set(set, s1[end - 1]))
		end--;
	return (ft_substr(s1, start, end - start));
}
/*
int	main(int ac, char **av)
{
	if (ac < 1)
		return (0);
	printf("%s\n", ft_strtrim(av[1], av[2]));
}*/
