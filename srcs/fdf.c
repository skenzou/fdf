/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/20 17:57:30 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>
static int g_xoffset;
static int g_yoffset;
static double g_iso_cte1;
static double g_iso_cte2;

void	print_map(t_map *map)
{
	int i;
	int j;

	j = 0;
	while (j < map->y)
	{
		i = 0;
		while (i < map->x)
		{
			printf("%*d", !i ? 0 : 3, map->board[j][i]);
			i++;
		}
		printf("\n");
		j++;
	}
}

int		handle_key(int keycode, void *param)
{
	t_fdf *fdf;

	fdf = (t_fdf *)param;
	if (keycode == 53)
		exit(0);
	if (keycode == 124)
	{
		g_xoffset += 30;
		mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
		draw(fdf);
	}
	if (keycode == 123)
	{
		g_xoffset -= 30;
		mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
		draw(fdf);
	}
	if (keycode == 125)
	{
		g_yoffset += 30;
		mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
		draw(fdf);
	}
	if (keycode == 126)
	{
		g_yoffset -= 30;
		mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
		draw(fdf);
	}
	printf("You pressed the key number: %d\n", keycode);
	return (1);
}

void    put_line(t_fdf *fdf, int x1, int y1, int x2,int y2)
{
    int		i;
    int		dx;
	int		dy;
    int		var;

	x2 = x2 - x1;
	y2 = y2 - y1;
	dx = x2 > 0 ? 1 : -1;
	dy = y2 > 0 ? 1 : -1;
	x2 = ABS(x2);
	y2 = ABS(y2);
    mlx_pixel_put(fdf->mlx_ptr,fdf->win_ptr, x1, y1, 0xFF0000) && (i = 1);
    var = (x2 > y2 ? x2 : y2) / 2;
    if (x2 > y2)
        while (i++ <= x2 && (x1 += dx) && (var += y2))
        {
			(var >= x2) && (y1 += dy);
            (var >= x2) && (var -= x2);
            mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, x1, y1, 0xFF0000);
        }
    else
        while (i++ <= y2 && (y1 += dy) && (var += x2))
        {
            (var >= y2) && (x1 += dx);
            (var >= y2) && (var -= y2);
            mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, x1, y1, 0xFF0000);
        }
}

void		draw(t_fdf *fdf)
{
	int i;
	int x2;
	int y2;
	int j;
	int z;
	int x;
	int y;

	j = 0;
	while (j < fdf->map->y)
	{
		i = 0;
		x = g_xoffset + (i * 20);
		y = g_yoffset + (j * 20);
		z = fdf->map->board[j][i];

		x = g_xoffset + (g_iso_cte1 * x - g_iso_cte2 * y);
		y = g_yoffset + (-z + (g_iso_cte1 / 2.0) * x + (g_iso_cte2 / 2.0) * y);
		i++;
		while (i < fdf->map->x)
		{
			x2 = g_xoffset + (i * 20);
			y2 = g_yoffset + (j * 20);
			z = fdf->map->board[j][i];

			x2 = g_xoffset + (g_iso_cte1 * x2 - g_iso_cte2 * y2);
			y2 = g_yoffset + (-z + (g_iso_cte1 / 2.0) * x2 + (g_iso_cte2 / 2.0) * y2);
			put_line(fdf, x, y, x2, y2);
			x = x2;
			y = y2;
			i++;
		}
		j++;
	}
	i = 0;
	while (i < fdf->map->x)
	{
		j = 0;
		x = g_xoffset + (i * 20);
		y = g_yoffset + (j * 20);
		z = fdf->map->board[j][i];

		x = g_xoffset + (g_iso_cte1 * x - g_iso_cte2 * y);
		y = g_yoffset + (-z + (g_iso_cte1 / 2.0) * x + (g_iso_cte2 / 2.0) * y);

		j++;
		while (j < fdf->map->y)
		{
			x2 = g_xoffset + (i * 20);
			y2 = g_yoffset + (j * 20);
			z = fdf->map->board[j][i];

			x2 = g_xoffset + (g_iso_cte1 * x2 - g_iso_cte2 * y2);
			y2 = g_yoffset + (-z + (g_iso_cte1 / 2.0) * x2 + (g_iso_cte2 / 2.0) * y2);
			put_line(fdf, x, y, x2, y2);
			x = x2;
			y = y2;
			j++;
		}
		i++;
	}
}

int		main(int argc, char **argv)
{
	t_fdf	*fdf;


	(void)argc;
	fdf = (t_fdf *)malloc(sizeof(t_fdf));
	fdf->mlx_ptr = mlx_init();
	fdf->win_ptr = mlx_new_window(fdf->mlx_ptr, 1000, 1000, "fdf");
	fdf->map = create_map(open(argv[1], O_RDONLY));
	print_map(fdf->map);
	g_xoffset = 200;
	g_yoffset = 100;
	mlx_key_hook(fdf->win_ptr, &handle_key, fdf);
	g_iso_cte1 = 0.5;
	g_iso_cte2 = 0.7;
	draw(fdf);
	mlx_loop(fdf->mlx_ptr);
	return (0);
}

// -------------------------------------------------------------------------
// void	print_point(t_list *list)
// {
// 	int			i;
// 	t_point		*point;
//
// 	i = 0;
// 	while (list)
// 	{
// 		point = (t_point*)list->content;
// 		printf("Le point numero %d x: %d y: %d z: %d\n", i++, point->x,
// 			point->y, point->z);
// 		list = list->next;
// 	}
// }

// t_point		*create_point(int x, int y, int z, int fd)
// {
// 	t_point *point;
//
// 	if (!(point = (t_point *)malloc(sizeof(t_point))))
// 	{
// 		close(fd);
// 		exit(1);
// 	}
// 	point->x = x;
// 	point->y = y;
// 	point->z = z;
// 	return (point);
// }

// // static	t_list	*line_to_points(str,)
// int		create_list(int fd, int *x, int *y, t_list **begin)
// {
// 	char		*str;
// 	char		*save;
// 	int			ret;
// 	t_list		*lst;
//
// 	while ((ret = get_next_line(fd, &str)) > 0)
// 	{
// 		*x = 0;
// 		save = str;
// 		while (str && *str)
// 		{
// 			if (ft_isdigit(*str) || *str == '-')
// 			{
// 				if (!(lst = ft_lstnew(create_point(
// 							(*x)++, *y, ft_atoi(str), fd), sizeof(t_point))))
// 					return (ft_lstdestroy(begin));
// 				ft_lstadd(begin, lst);
// 				while (ft_isdigit(*(str + 1)))
// 					str++;
// 			}
// 			str++;
// 		}
// 		ft_strdel(&save);
// 		(*y)++;
// 	}
// 	ret ? ft_lstdestroy(begin) : ft_lstrev(begin);
// 	return (1);
// }
//
// // static	void	destroy_map(int **map, int x, int y)
// // {
// //
// // }
// static	int		**create_map(int fd)
// {
// 	t_list	*list;
// 	int		**map;
// 	int		*tab;
// 	t_point *point;
// 	int		x;
// 	int		y;
// 	int		i;
// 	int		j;
//
// 	y = 0;
// 	j = 0;
// 	list = NULL;
// 	create_list(fd, &x, &y, &list);
// 	map = (int **)malloc(sizeof(int *) * y);
// 	while (map && j < y)
// 	{
// 		i = 0;
// 		tab = (int *)malloc(sizeof(int) * x);
// 		while (list && i < x && (point = (t_point*)list->content))
// 		{
// 			tab[i++] = point->z;
// 			list = list->next;
// 		}
// 		map[j++] = tab;
// 	}
// 	//print_map(map, x, y);
// 	return (map);
// }
