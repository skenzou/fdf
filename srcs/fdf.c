/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/13 07:47:06 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>

void 	print_point(t_list *list)
{
	int i;
	t_point *point;

	i = 0;
	while (list)
	{
		point = (t_point*)list->content;
			printf("Le point numero %d x: %d y: %d z: %d", i++, point->x, point->y, point->z);
			list = list->next;
	}
}

t_point		*create_point(int x, int y, int z)
{
	t_point *point;

	if (!(point = (t_point *)malloc(sizeof(t_point))))
		return (NULL);
	point->x = x;
	point->y = y;
	point->z = z;
	return (point);
}

int main (int argc, char **argv)
{
	int x;
	int y;
	char *str;
	int ret;
	int fd;
	t_list *list;

	(void)argc;
	fd = open(argv[1], O_RDONLY);
	list = NULL;
	y = 0;
	while ((ret = get_next_line(fd, &str)) > 0)
	{
		x = 0;
		while (str && str[x])
		{
			if (ft_isdigit(str[x]))
				ft_lstpushback(&list, ft_lstnew(create_point(x, y, str[x] - 48), sizeof(t_point)));
			x++;
		}
		y++;
	}
	print_point(list);
	return (0);
}
