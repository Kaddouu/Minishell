/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:30:06 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/23 16:30:11 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line_bonus.h"

int	ft_strlen_gnl(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*newtab;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	newtab = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!newtab)
	{
		free(s1);
		return (0);
	}
	if (s1 != NULL)
	{	
		while (s1[i])
			newtab[j++] = s1[i++];
	}
	i = 0;
	while (s2[i])
		newtab[j++] = s2[i++];
	newtab[j] = '\0';
	free(s1);
	return (newtab);
}

int	ft_strchr_gnl(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}
