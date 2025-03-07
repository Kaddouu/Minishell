/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:53:47 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/03/07 17:26:57 by ysaadaou         ###   ########.fr       */
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

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_strjoin_char_free(char *s, char c)
{
	char	*result;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	result = (char *)malloc(len + 2);
	if (!result)
	{
		free(s);
		return (NULL);
	}
	ft_strcpy(result, s);
	result[len] = c;
	result[len + 1] = '\0';
	free(s);
	return (result);
}

char *ft_strjoin_three(char *s1, char *s2, char *s3)
{
    char *tmp;
    char *result;

    tmp = ft_strjoin(s1, s2);
    if (!tmp)
        return NULL;
    result = ft_strjoin(tmp, s3);
    free(tmp);
    return result;
}
