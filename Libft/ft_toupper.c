/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:33:24 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/08 16:34:48 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
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
	if (c >= 'A' && c <= 'Z')
	{
		printf("\nCe carcatere est deja en majusule\n");
		return (0);
	}
	else
	{
		res = ft_toupper(c);
		printf("\nLe caractere en majuscule : %c\n", res);
	}
	return (0);
}*/
