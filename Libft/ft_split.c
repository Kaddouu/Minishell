/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 16:34:10 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/12 16:34:19 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	nb_words(char const *s, char c)
{
	size_t	count;
	size_t	i;
	int		in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			if (in_word != 1)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		i++;
	}
	return (count);
}

static char	*fill_my_tab(char const *s, char c, size_t *i)
{
	char	*minitab;
	size_t	j;
	size_t	len;

	j = 0;
	len = 0;
	while (s[*i] == c)
		(*i)++;
	while (s[*i + len] && s[*i + len] != c)
		len++;
	minitab = malloc(sizeof(char) * (len + 1));
	if (!minitab)
		return (NULL);
	while (s[*i] && s[*i] != c)
		minitab[j++] = s[(*i)++];
	minitab[j] = '\0';
	return (minitab);
}

void	free_bigtab(char **bigtab, size_t minitab)
{
	size_t	i;

	i = 0;
	while (i < minitab)
	{
		free(bigtab[i]);
		i++;
	}
	free(bigtab);
}

static char	**fill_bigtab(char const *s, char c, size_t minitab, size_t intab)
{
	char	**bigtab;

	bigtab = malloc(sizeof(char *) * (nb_words(s, c) + 1));
	if (!bigtab)
		return (0);
	while (s[intab])
	{
		while (s[intab] == c)
			intab++;
		if (s[intab] == '\0')
			break ;
		bigtab[minitab] = fill_my_tab(s, c, &intab);
		if (!bigtab[minitab])
		{
			free_bigtab(bigtab, minitab);
			return (NULL);
		}
		minitab++;
	}
	bigtab[minitab] = NULL;
	return (bigtab);
}

char	**ft_split(char const *s, char c)
{
	char	**bigtab;
	size_t	minitab;
	size_t	intab;

	minitab = 0;
	intab = 0;
	if (!s)
		return (NULL);
	bigtab = fill_bigtab(s, c, minitab, intab);
	return (bigtab);
}
/*

	bigtab = malloc(sizeof(char *) * (nb_words(s, c) + 1));
	if (!bigtab)
		return (0);
	while (s[intab])
	{
		while (s[intab] == c)
			intab++;
		if (s[intab] == '\0')
			break ;
		bigtab[minitab++] = fill_my_tab(s, c, intab);
		intab += in_words(s, c, intab);
	}
	bigtab[minitab] = NULL;

int	main(int ac, char **av)
{
	if (ac < 1)
		return (0);
	//char **bigtab = ft_split("Je suis un test de 42", ' ');
	//char **bigtab = ft_split("split  ||this|for|me|||||!|||| || |", '|');
	//char **bigtab = ft_split("\0aa\0bbb", '\0');
	//char **bigtab = ft_split(av[1], av[2]);
	
	
	char **temp = bigtab;
	
	while (*temp)
	{
		printf("%s\n", *temp);
		free(*temp);
		temp++;
	}
	free(bigtab);
	return (0);
}*/
