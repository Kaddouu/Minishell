/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:44:51 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/10 15:47:19 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}
/*
int	main()
{	
	printf("%p\n", ft_strchr("teste", 's'));
	printf("%p\n\n", strchr("teste", 's'));
	
	printf("%p\n", ft_strrchr("teste", 'e'));
	printf("%p\n\n", strrchr("teste", 'e'));
		
	printf("%p\n", ft_strchr("teste", '\0'));
	printf("%p\n\n", strchr("teste", '\0'));
	
	printf("%p\n", ft_strchr("teste", 1024));
	printf("%p\n\n", strchr("teste", 1024));
	
	printf("%p\n", ft_strchr("teste", 1025));
	printf("%p\n\n", strchr("teste", 1025));
	
	printf("%p\n", ft_strchr("teste", 1028));
	printf("%p\n\n", strchr("teste", 1028));
	
	printf("%p\n", ft_strchr("teste", 1030));
	printf("%p\n\n", strchr("teste", 1030));
	
	//printf("%p\n", ft_strrchr(((void*)0), '\0'));
	//printf("%p\n", strrchr(((void*)0), '\0'));
}*/
