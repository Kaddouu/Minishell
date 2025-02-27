/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 15:31:08 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/16 15:33:28 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*supp;

	if (!lst)
		return ;
	while (*lst)
	{
		supp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = supp;
	}
	*lst = NULL;
}
