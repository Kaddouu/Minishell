/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:56:54 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/13 17:57:25 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	size_to_allocate(long int nb)
{
	size_t	count;

	count = 0;
	if (nb < 0)
	{
		count++;
		nb = -nb;
	}
	if (nb == 0)
	{
		count++;
		return (count);
	}
	while (nb > 0)
	{
		count++;
		nb = nb / 10;
	}
	return (count);
}

static char	*convert_swap_str(char *my_string, long int nb, size_t i)
{
	size_t	j;
	char	tmp;

	j = 0;
	while (nb > 0)
	{
		my_string[i++] = nb % 10 + '0';
		nb = nb / 10;
	}
	my_string[i] = '\0';
	if (my_string[0] == '-')
		j++;
	while (j < i)
	{
		tmp = my_string[--i];
		my_string[i] = my_string[j];
		my_string[j++] = tmp;
	}
	return (my_string);
}

char	*ft_itoa(int n)
{
	size_t		i;
	char		*my_string;
	long int	nb;

	i = 0;
	nb = n;
	my_string = malloc(sizeof(char) * size_to_allocate(nb) + 1);
	if (!my_string)
		return (0);
	if (nb < 0)
	{
		my_string[i++] = '-';
		nb = -nb;
	}
	if (nb == 0)
	{
		my_string[i++] = 48;
		my_string[i] = '\0';
		return (my_string);
	}
	my_string = convert_swap_str(my_string, nb, i);
	return (my_string);
}
/*
void	ft_putstr(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		write (1, &str[i], 1);
		i++;
	}
}

int	main(int ac, char **av)
{
	if (ac < 1)
		return (0);
	ft_putstr(ft_itoa(ft_atoi(av[1])));
	write (1, "\n", 1);
	return (0);
}*/
