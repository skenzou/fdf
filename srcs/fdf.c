/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/14 08:33:59 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>

void	print_point(t_list *list)
{
	int			i;
	t_point		*point;

	i = 0;
	while (list)
	{
		point = (t_point*)list->content;
		printf("Le point numero %d x: %d y: %d z: %d\n", i++, point->x,
			point->y, point->z);
		list = list->next;
	}
}

void	print_map(int **map, int x, int y)
{
	int i;
	int j;

	j = 0;
	while (j < y)
	{
		i = 0;
		while (i < x)
		{
			printf("%*d", !i ? 0 : 3, map[j][i]);
			i++;
		}
		printf("\n");
		j++;
	}
}

t_point		*create_point(int x, int y, int z, int fd)
{
	t_point *point;

	if (!(point = (t_point *)malloc(sizeof(t_point))))
	{
		close(fd);
		exit(1);
	}
	point->x = x;
	point->y = y;
	point->z = z;
	return (point);
}

void		create_list(int fd, int *x, int *y, t_list **begin)
{
	char		*str;
	char		*save;
	int			ret;
	t_list		*lst;

	while ((ret = get_next_line(fd, &str)) > 0)
	{
		*x = 0;
		save = str;
		while (str && *str)
		{
			if (ft_isdigit(*str) || *str == '-')
			{
				if (!(lst = ft_lstnew(create_point(
							(*x)++, *y, ft_atoi(str), fd), sizeof(t_point))))
				{
					ft_lstdestroy(begin);
					return ;
				}
				ft_lstadd(begin, lst);
				while (ft_isdigit(*(str + 1)))
					str++;
			}
			str++;
		}
		ft_strdel(&save);
		(*y)++;
	}
	ret ? ft_lstdestroy(begin) : ft_lstrev(begin);
}
// static	void	destroy_map(int **map, int x, int y)
// {
//
// }
static	int		**create_map(int fd)
{
	t_list	*list;
	int		**map;
	int		*tab;
	t_point *point;
	int		x;
	int		y;
	int		i;
	int		j;

	y = 0;
	j = 0;
	list = NULL;
	create_list(fd, &x, &y, &list);
	map = (int **)malloc(sizeof(int *) * y);
	while (map && j < y)
	{
		i = 0;
		tab = (int *)malloc(sizeof(int) * x);
		while (list && i < x && (point = (t_point*)list->content))
		{
			tab[i++] = point->z;
			list = list->next;
		}
		map[j++] = tab;
	}
	print_map(map, x, y);
	return (map);
}

int		main(int argc, char **argv)
{
	int	**map;

	(void)argc;
	map = create_map(open(argv[1], O_RDONLY));
	return (0);
}
