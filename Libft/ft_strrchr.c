/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 16:04:23 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/10 16:04:37 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
/*
int	main()
{	
	printf("%p\n", ft_strrchr("teste", 's'));
	printf("%p\n\n", strrchr("teste", 's'));
	
	printf("%p\n", ft_strrchr("teste", 1024));
	printf("%p\n\n", strrchr("teste", 1024));
	
	printf("%p\n", ft_strrchr("teste", 1028));
	printf("%p\n\n", strrchr("teste", 1028));
	
	printf("%p\n", ft_strrchr("teste", 1030));
	printf("%p\n\n", strrchr("teste", 1030));
	
	//printf("%p\n", ft_strrchr(((void*)0), '\0'));
	//printf("%p\n", strrchr(((void*)0), '\0'));
}*/
