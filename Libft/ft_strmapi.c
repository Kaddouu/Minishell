/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:15:07 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/14 14:18:49 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	char	*restab;

	i = 0;
	if (!s)
		return (0);
	restab = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!restab)
		return (0);
	while (s[i])
	{
		restab[i] = f(i, s[i]);
		i++;
	}
	restab[i] = '\0';
	return (restab);
}
