/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 10:27:35 by midrissi          #+#    #+#             */
/*   Updated: 2018/10/13 09:53:06 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del) (void *, size_t))
{
	t_list *list;

	list = *alst;
	while (list)
	{
		*alst = list->next;
		del(list->content, list->content_size);
		free(list);
		list = *alst;
	}
	*alst = NULL;
}
