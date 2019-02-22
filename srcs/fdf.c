/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/22 09:25:04 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>

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
	fdf->xoffset = keycode == 124 ? fdf->xoffset + 30 : fdf->xoffset;
	fdf->xoffset = keycode == 123 ? fdf->xoffset - 30 : fdf->xoffset;
	fdf->yoffset = keycode == 125 ? fdf->yoffset + 30 : fdf->yoffset;
	fdf->yoffset = keycode == 126 ? fdf->yoffset - 30 : fdf->yoffset;
	fdf->zoom = keycode == 69 ? fdf->zoom + 1 : fdf->zoom;
	fdf->zoom = keycode == 78 ? fdf->zoom - 1 : fdf->zoom;
	fdf->zoom = keycode == 18 ? fdf->zoom + 1 : fdf->zoom;
	fdf->zoom = keycode == 19 ? fdf->zoom - 1 : fdf->zoom;
	fdf->altitude = keycode == 13 ? fdf->altitude + 1 : fdf->altitude;
	fdf->altitude = keycode == 1 ? fdf->altitude - 1 : fdf->altitude;
	if (keycode == 12)
		fdf->rasterise = fdf->rasterise == &rasterise_iso
										? &rasterise_par : &rasterise_iso;
	process(fdf);
	printf("You pressed the key number: %d\n", keycode);
	return (1);
}

void	put_line(t_fdf *fdf, int x1, int y1, int x2,int y2)
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
	if (x1 < WIN_WIDTH && x1 > 0 && y1 < WIN_HEIGHT && y1 > 0)
	*(int *)(fdf->img->data + ((x1 + y1 * WIN_WIDTH) * fdf->img->bpp)) = mlx_get_color_value(fdf->mlx_ptr, 0xFF0000);
	i = 1;
	var = (x2 > y2 ? x2 : y2) / 2;
	if (x2 > y2)
		while (i++ <= x2 && (x1 += dx) && (var += y2))
		{
			(var >= x2) && (y1 += dy);
			(var >= x2) && (var -= x2);
			if (x1 < WIN_WIDTH && x1 > 0 && y1 < WIN_HEIGHT && y1 > 0)
			*(int *)(fdf->img->data + ((x1 + y1 * WIN_WIDTH) * fdf->img->bpp)) = mlx_get_color_value(fdf->mlx_ptr, 0xFF0000);
		}
	else
		while (i++ <= y2 && (y1 += dy) && (var += x2))
		{
			(var >= y2) && (x1 += dx);
			(var >= y2) && (var -= y2);
			if (x1 < WIN_WIDTH && x1 > 0 && y1 < WIN_HEIGHT && y1 > 0)
			*(int *)(fdf->img->data + ((x1 + y1 * WIN_WIDTH) * fdf->img->bpp)) = mlx_get_color_value(fdf->mlx_ptr, 0xFF0000);
		}
}

void	create_image(t_fdf *fdf)
{
	if (!(fdf->img = (t_image *)malloc(sizeof(t_image))))
		exit (1);
	fdf->img->ptr = mlx_new_image(fdf->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	fdf->img->data = mlx_get_data_addr(fdf->img->ptr,
			&fdf->img->bpp, &fdf->img->sizeline, &fdf->img->endian);
	fdf->img->bpp /= 8;
}

void	rasterise_iso(t_fdf *fdf, int *x, int *y, int z)
{
	double cte1;
	double cte2;

	cte1 = 0.5;
	cte2 = 0.5;
	fdf->zoom = fdf->zoom < 0 ? 0 : fdf->zoom;
	*x = fdf->xoffset + ((*x) * fdf->zoom);
	*y = fdf->yoffset + ((*y) * fdf->zoom);
	z = z * fdf->altitude;
	*x = fdf->xoffset + (cte1 * (*x) - cte2 * (*y));
	*y = fdf->yoffset + (-z + (cte1 / 2.0) * (*x) + (cte2 / 2.0) * (*y));
}

void 	rasterise_par(t_fdf *fdf, int *x, int *y, int z)
{
	double cte;

	cte = 0.5;
	fdf->zoom = fdf->zoom < 0 ? 0 : fdf->zoom;
	*x = fdf->xoffset + ((*x) * fdf->zoom);
	*y = fdf->yoffset + ((*y) * fdf->zoom);
	z = z * fdf->altitude;
	*x = *x + cte * z;
	*y = *y + (cte / 2.0) * z;
}

void		draw(t_fdf *fdf)
{
	int i;
	int j;
	int x1;
	int y1;
	int x2;
	int y2;

	j = -1;
	while (++j < fdf->map->y && !(i = 0))
		while (i < fdf->map->x)
		{
			x1 = i;
			y1 = j;
			x2 = i == fdf->map->x - 1 ? i : i + 1;
			y2 = j;
			fdf->rasterise(fdf, &x1, &y1, fdf->map->board[y1][x1]);
			fdf->rasterise(fdf, &x2, &y2, fdf->map->board[y2][x2]);
			put_line(fdf, x1, y1, x2, y2);
			x2 = i;
			y2 = j == fdf->map->y - 1 ? j : j + 1;
			fdf->rasterise(fdf, &x2, &y2, fdf->map->board[y2][x2]);
			put_line(fdf, x1, y1, x2, y2);
			i++;
		}
}

int		scale_zoom(t_fdf *fdf)
{
	int zoom;

	zoom = 1;
	while (fdf->map->x * zoom < WIN_WIDTH)
		zoom++;
	return (zoom - 1);
}

t_fdf			*init_fdf(int fd)
{
		t_fdf	*fdf;

		if (!(fdf = (t_fdf *)malloc(sizeof(t_fdf))) || fd == -1)
			exit(1);
		fdf->mlx_ptr = mlx_init();
		fdf->win_ptr = mlx_new_window(fdf->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "fdf");
		fdf->map = create_map(fd);
		if (!fdf->mlx_ptr || !fdf->win_ptr || close(fd))
			exit(1);
		fdf->xoffset = 500;
		fdf->yoffset = 100;
		mlx_key_hook(fdf->win_ptr, &handle_key, fdf);
		fdf->zoom = scale_zoom(fdf);
		fdf->altitude = 1;
		fdf->rasterise = &rasterise_iso;
		fdf->img = NULL;
		return (fdf);
}

void		process(t_fdf *fdf)
{
	mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
	if (fdf->img)
		mlx_destroy_image(fdf->mlx_ptr, fdf->img->ptr);
	create_image(fdf);
	draw(fdf);
	mlx_put_image_to_window(fdf->mlx_ptr, fdf->win_ptr, fdf->img->ptr, 0 , 0);
}

int		main(int argc, char **argv)
{
	t_fdf	*fdf;

	argc != 2 ? exit(1) : 0;
	fdf = init_fdf(open(argv[1], O_RDONLY));
	process(fdf);
	mlx_loop(fdf->mlx_ptr);
	return (0);
}










// void		draw(t_fdf *fdf)
// {
// 	int i;
// 	int x2;
// 	int y2;
// 	int j;
// 	int z;
// 	int x;
// 	int y;
// 	double cte1;
// 	double cte2;
//
// 	j = 0;
// 	cte2 = 0.7;
// 	cte1 = 0.5;
// 	while (j < fdf->map->y)
// 	{
// 		i = 0;
// 		x = fdf->xoffset + (i * fdf->zoom);
// 		y = fdf->yoffset + (j * fdf->zoom);
// 		z = fdf->map->board[j][i] * fdf->altitude;
//
// 		x = fdf->xoffset + (cte1 * x - cte2 * y);
// 		y = fdf->yoffset + (-z + (cte1 / 2.0) * x + (cte2 / 2.0) * y);
// 		i++;
// 		while (i < fdf->map->x)
// 		{
// 			x2 = fdf->xoffset + (i * fdf->zoom);
// 			y2 = fdf->yoffset + (j * fdf->zoom);
// 			z = fdf->map->board[j][i] * fdf->altitude;
//
// 			x2 = fdf->xoffset + (cte1 * x2 - cte2 * y2);
// 			y2 = fdf->yoffset + (-z + (cte1 / 2.0) * x2 + (cte2 / 2.0) * y2);
// 			put_line(fdf, x, y, x2, y2);
// 			x = x2;
// 			y = y2;
// 			i++;
// 		}
// 		j++;
// 	}
// 	i = 0;
// 	while (i < fdf->map->x)
// 	{
// 		j = 0;
// 		x = fdf->xoffset + (i * fdf->zoom);
// 		y = fdf->yoffset + (j * fdf->zoom);
// 		z = fdf->map->board[j][i] * fdf->altitude;
//
// 		x = fdf->xoffset + (cte1 * x - cte2 * y);
// 		y = fdf->yoffset + (-z + (cte1 / 2.0) * x + (cte2 / 2.0) * y);
//
// 		j++;
// 		while (j < fdf->map->y)
// 		{
// 			x2 = fdf->xoffset + (i * fdf->zoom);
// 			y2 = fdf->yoffset + (j * fdf->zoom);
// 			z = fdf->map->board[j][i] * fdf->altitude;
//
// 			x2 = fdf->xoffset + (cte1 * x2 - cte2 * y2);
// 			y2 = fdf->yoffset + (-z + (cte1 / 2.0) * x2 + (cte2 / 2.0) * y2);
// 			put_line(fdf, x, y, x2, y2);
// 			x = x2;
// 			y = y2;
// 			j++;
// 		}
// 		i++;
// 	}
// }
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
