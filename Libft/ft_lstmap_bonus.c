/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:22:55 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/17 16:23:26 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_element;
	t_list	*list;
	void	*new_content;

	if (!lst || !f || !del)
		return (NULL);
	list = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		new_element = ft_lstnew(new_content);
		if (!new_element)
		{
			del(new_content);
			ft_lstclear(&list, del);
			return (NULL);
		}
		ft_lstadd_back(&list, new_element);
		lst = lst->next;
	}
	return (list);
}
