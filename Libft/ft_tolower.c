/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:35:23 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/08 16:35:24 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	else
		return (c);
}
/*
int main()
{
	char c;
	char res;

	printf("Entrez un caractere : ");
	scanf("%c", &c);
	while ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z'))
	{
		printf("\nLe caractere saisie n'est pas une lettre\n");
		printf("\nVeuillez saisir une lettre : ");
		scanf(" %c", &c);
	}
	if (c >= 'a' && c <= 'z')
	{
		printf("\nCe carcatere est deja en minuscule\n");
		return (0);
	}
	else
	{
		res = ft_tolower(c);
		printf("\nLe caractere en minuscule : %c\n", res);
	}
	return (0);
}*/
