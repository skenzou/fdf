/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/25 03:35:10 by midrissi         ###   ########.fr       */
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
	fdf->xoffset = keycode == RIGHTARROW ? fdf->xoffset + 30 : fdf->xoffset;
	fdf->xoffset = keycode == LEFTARROW ? fdf->xoffset - 30 : fdf->xoffset;
	fdf->yoffset = keycode == DOWNARROW ? fdf->yoffset + 30 : fdf->yoffset;
	fdf->yoffset = keycode == UPARROW ? fdf->yoffset - 30 : fdf->yoffset;
	fdf->zoom = keycode == PLUS ? fdf->zoom + 1 : fdf->zoom;
	fdf->zoom = keycode == MINUS ? fdf->zoom - 1 : fdf->zoom;
	fdf->zoom = keycode == 18 ? fdf->zoom + 1 : fdf->zoom;
	fdf->zoom = keycode == 19 ? fdf->zoom - 1 : fdf->zoom;
	fdf->altitude = keycode == WKEY ? fdf->altitude + 1 : fdf->altitude;
	fdf->altitude = keycode == SKEY ? fdf->altitude - 1 : fdf->altitude;
	if (keycode == QKEY)
	{
		fdf->rasterise = fdf->rasterise == &rasterise_iso
										? &rasterise_par : &rasterise_iso;
		fdf->zoom = fdf->rasterise == &rasterise_par
										? fdf->zoom / 2 : fdf->zoom * 2;
		center(fdf);
	}
	process(fdf);
	printf("You pressed the key number: %d\n", keycode);
	return (1);
}

void	put_line(t_fdf *fdf, t_point p1, t_point p2, int border, int color)
{
	int		i;
	t_point inc;
	int		var;

	p2 = (t_point){.x = p2.x - p1.x, .y = p2.y - p1.y};
	inc = (t_point){.x = p2.x > 0 ? 1 : -1, .y = p2.y > 0 ? 1 : -1};
	p2 = (t_point){.x = abs(p2.x), .y = abs(p2.y)};
	if (p1.y < 0 || p2.y < 0)
		printf("point1 |%d, %d|   |   point2 |%d, %d|\n", p1.x, p1.y, p2.x, p2.y);
	if (border || (p1.x < DRAW_WIDTH && p1.x > 4 && p1.y < DRAW_HEIGHT && p1.y > 4))
		*(int *)(fdf->img->data + ((p1.x + p1.y * WIN_WIDTH) *
		fdf->img->bpp)) = mlx_get_color_value(fdf->mlx_ptr, color);
	var = (p2.x > p2.y ? p2.x : p2.y) / 2;
	if ((i = 1) && p2.x > p2.y)
		while (i++ <= p2.x && (p1.x += inc.x) && (var += p2.y))
		{
			(var >= p2.x) && (p1.y += inc.y);
			(var >= p2.x) && (var -= p2.x);
			if (border || (p1.x < DRAW_WIDTH && p1.x > 4 && p1.y < DRAW_HEIGHT && p1.y > 4))
				*(int *)(fdf->img->data + ((p1.x + p1.y * WIN_WIDTH) *
				fdf->img->bpp)) = mlx_get_color_value(fdf->mlx_ptr, color);
		}
	else
		while (i++ <= p2.y && (p1.y += inc.y) && (var += p2.x))
		{
			(var >= p2.y) && (p1.x += inc.x);
			(var >= p2.y) && (var -= p2.y);
			if (border || (p1.x < DRAW_WIDTH && p1.x > 4 && p1.y < DRAW_HEIGHT && p1.y > 4))
				*(int *)(fdf->img->data + ((p1.x + p1.y * WIN_WIDTH) *
				fdf->img->bpp)) = mlx_get_color_value(fdf->mlx_ptr, color);
		}
}

void	create_image(t_fdf *fdf)
{
	if (!(fdf->img = (t_image *)malloc(sizeof(t_image))))
		exit(1);
	fdf->img->ptr = mlx_new_image(fdf->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	fdf->img->data = mlx_get_data_addr(fdf->img->ptr,
			&fdf->img->bpp, &fdf->img->sizeline, &fdf->img->endian);
	fdf->img->bpp /= 8;
}

t_point	rasterise_iso(t_fdf *fdf, t_point p, int z)
{
	double cte1;
	double cte2;

	cte1 = 0.5;
	cte2 = 0.5;
	fdf->zoom = fdf->zoom < 0 ? 0 : fdf->zoom;
	p.x = fdf->xoffset + (p.x * fdf->zoom);
	p.y = fdf->yoffset + (p.y * fdf->zoom);
	z = z * fdf->altitude;
	p.x = fdf->xoffset + (cte1 * p.x - cte2 * p.y);
	p.y = fdf->yoffset + (-z + (cte1 / 2.0) * p.x + (cte2 / 2.0) * p.y);
	return (p);
}

t_point	rasterise_par(t_fdf *fdf, t_point p, int z)
{
	double cte;

	cte = 0.5;
	fdf->zoom = fdf->zoom < 0 ? 0 : fdf->zoom;
	p.x = fdf->xoffset + (p.x * fdf->zoom);
	p.y = fdf->yoffset + (p.y * fdf->zoom);
	z = z * fdf->altitude;
	p.x = p.x + cte * z;
	p.y = p.y + (cte / 2.0) * z;
	return (p);
}

void		put_legend(t_fdf *fdf)
{
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 10, ROYALBLUE,
	"1 or +      - zoom");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 50, ROYALBLUE,
	"2 or -      - unzoom");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 90, ROYALBLUE,
	"W           - increase altitude");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 130, ROYALBLUE,
	"S           - decrease altitude");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 180, ROYALBLUE,
	"Q           - change projection");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 220, ROYALBLUE,
	"left arrow  - move left");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 260, ROYALBLUE,
	"right arrow - move right");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 300, ROYALBLUE,
	"up arrow    - move up");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 340, ROYALBLUE,
	"down arrow  - move down");
}

void		put_borders(t_fdf *fdf)
{
	put_line(fdf, (t_point){.x = 4, .y = 1300},
					(t_point){.x = 2000, .y = 1300}, 1, ROYALBLUE);
	put_line(fdf, (t_point){.x = 2000, .y = 1300},
					(t_point){.x = 2000, .y = 4}, 1, ROYALBLUE);
	put_line(fdf, (t_point){.x = 4, .y = 1300},
					(t_point){.x = 4, .y = 4}, 1, ROYALBLUE);
	put_line(fdf, (t_point){.x = 4, .y = 4},
					(t_point){.x = 2000, .y = 4}, 1, ROYALBLUE);
}

void		draw(t_fdf *fdf)
{
	int		i;
	int		j;
	t_point p1;
	t_point p2;

	j = -1;
	while (++j < fdf->map->y && !(i = 0))
		while (i < fdf->map->x)
		{
			p1 = (t_point){.x = i, .y = j};
			p2 = (t_point){.x = i == fdf->map->x - 1 ? i : i + 1, .y = j};
			p1 = fdf->rasterise(fdf, p1, fdf->map->board[p1.y][p1.x]);
			p2 = fdf->rasterise(fdf, p2, fdf->map->board[p2.y][p2.x]);
			put_line(fdf, p1, p2, 0, GOLD);
			p2 = (t_point){.x = i, .y = j == fdf->map->y - 1 ? j : j + 1};
			p2 = fdf->rasterise(fdf, p2, fdf->map->board[p2.y][p2.x]);
			put_line(fdf, p1, p2, 0, GOLD);
			i++;
		}
	put_borders(fdf);
}

void		center(t_fdf *fdf)
{
	t_point p1;
	t_point p2;

	p1 = fdf->rasterise(fdf, (t_point){.x = 0, .y = 0}, fdf->map->board[0][0]);
	p2 = fdf->rasterise(fdf, (t_point){.x = 0, .y = fdf->map->y - 1},
										fdf->map->board[fdf->map->y - 1][0]);
	printf("x2: %d\n", 10);
	// ft_printf("fdf->xoffset: %d\n", fdf->xoffset);
	// ft_printf("fdf->yoffset: %d\n", fdf->yoffset);
}

void		scale_zoom(t_fdf *fdf)
{
	int zoom;

	zoom = 1;
	while (fdf->map->x * zoom < WIN_WIDTH)
		zoom++;
	fdf->zoom = zoom - (zoom / 5);
}

int				handle_mouse(int button, int x, int y, void *param)
{
	t_fdf *fdf;

	fdf = (t_fdf *)param;
	(void)button;
	printf("I clicked on point: [%d, %d]\n", x, y);
	return (1);
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
	fdf->xoffset = 620;
	fdf->yoffset = 100;
	mlx_key_hook(fdf->win_ptr, &handle_key, fdf);
	mlx_mouse_hook(fdf->win_ptr, &handle_mouse, fdf);
	fdf->altitude = 3;
	fdf->rasterise = &rasterise_iso;
	fdf->img = NULL;
	scale_zoom(fdf);
	center(fdf);
	return (fdf);
}

void		process(t_fdf *fdf)
{
	mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
	if (fdf->img)
		mlx_destroy_image(fdf->mlx_ptr, fdf->img->ptr);
	create_image(fdf);
	draw(fdf);
	ft_printf("fdf->xoffset: %d\n", fdf->xoffset);
	ft_printf("fdf->yoffset: %d\n", fdf->yoffset);
	mlx_put_image_to_window(fdf->mlx_ptr, fdf->win_ptr, fdf->img->ptr, 0, 0);
	put_legend(fdf);
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
// 	return (mapROYALBLUE
