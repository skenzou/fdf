/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/26 19:12:05 by midrissi         ###   ########.fr       */
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

int		handle_key(int keycode, t_fdf *fdf)
{
	keycode == ESCAPE ? exit(0) : 0;
	(keycode == RIGHTARROW || keycode == DKEY) && (fdf->xoffset += 30);
	(keycode == LEFTARROW || keycode == AKEY) && (fdf->xoffset -= 30);
	(keycode == UPARROW || keycode == WKEY) && (fdf->yoffset -= 30);
	(keycode == DOWNARROW || keycode == SKEY) && (fdf->yoffset += 30);
	(keycode == PLUS || keycode == 18) && (fdf->altitude++);
	(keycode == MINUS || keycode == 19) && (fdf->altitude--);
	if (keycode == QKEY || keycode == SPACE)
	{
		fdf->rasterise = fdf->rasterise == &rasterise_iso
										? &rasterise_par : &rasterise_iso;
		fdf->zoom = fdf->rasterise == &rasterise_par
										? fdf->zoom / 2 : fdf->zoom * 2;
		center(fdf);
	}
	process(fdf);
	return (1);
}

int		get_light(int start, int end, double percentage)
{
	return ((int)((1 - percentage) * start + percentage * end));
}

double		percent(int start, int end, int curr)
{
	double placement;
	double distance;

	placement = curr - start;
	distance = end - start;
	return (!distance ? 1.0 : (placement / distance));
}

int		get_color(t_point current, t_point start, t_point end, t_point d)
{
	int		red;
	int		green;
	int		blue;
	double	percentage;

	if (current.color == end.color)
		return (current.color);
	if (d.x > d.y)
		percentage = percent(start.x, end.x, current.x);
	else
		percentage = percent(start.y, end.y, current.y);
	red = get_light((start.color >> 16) & 0xFF,
			(end.color >> 16) & 0xFF, percentage);
	green = get_light((start.color >> 8) & 0xFF,
			(end.color >> 8) & 0xFF, percentage);
	blue = get_light(start.color & 0xFF, end.color & 0xFF, percentage);
	return ((red << 16) | (green << 8) | blue);
}

int	put_pixel_img(t_fdf *fdf, t_point p, int border)
{
	int offset;
	int color;

	offset = ((p.x + p.y * WIN_WIDTH) * fdf->img->bpp);
	color = mlx_get_color_value(fdf->mlx_ptr, p.color);
	if (border || (p.x < DRAW_WIDTH && p.x > 4 && p.y < DRAW_HEIGHT && p.y > 4))
		*(int *)(fdf->img->data + offset) = color;
	return (1);
}

void	put_hor(t_fdf *fdf, t_point p1, t_point p2, t_point d)
{
	int i;
	int var;
	t_point inc;
	t_point curr;

	curr = (t_point){.x = p1.x, .y = p1.y, .color = p1.color};
	inc = (t_point){.x = p2.x - p1.x > 0 ? 1 : -1, .y = p2.y - p1.y > 0 ? 1 : -1};
	i = 1;
	var = (d.x > d.y ? d.x : d.y) / 2;
	while (i++ <= d.x && (var += d.y))
	{
		curr.x += inc.x;
		(var >= d.x) && (curr.y += inc.y);
		(var >= d.x) && (var -= d.x);
		(curr.color = get_color(curr, p1, p2, d)) && (put_pixel_img
																												(fdf, curr, p1.border));
	}
}

void	put_ver(t_fdf *fdf, t_point p1, t_point p2, t_point d)
{
	int i;
	int var;
	t_point inc;
	t_point curr;

	curr = (t_point){.x = p1.x, .y = p1.y, .color = p1.color};
	inc = (t_point){.x = p2.x - p1.x > 0 ? 1 : -1, .y = p2.y - p1.y > 0 ? 1 : -1};
	i = 1;
	var = (d.x > d.y ? d.x : d.y) / 2;
	while (i++ <= d.y && (var += d.x))
	{
		curr.y += inc.y;
		(var >= d.y) && (curr.x += inc.x);
		(var >= d.y) && (var -= d.y);
		(curr.color = get_color(curr, p1, p2, d)) && (put_pixel_img
																												(fdf, curr, p1.border));
	}
}

void	put_line(t_fdf *fdf, t_point p1, t_point p2)
{
	t_point d;

	d = (t_point){.x = p2.x - p1.x, .y = p2.y - p1.y};
	d = (t_point){.x = abs(d.x), .y = abs(d.y)};
	(p1.color = get_color(p1, p1, p2, d)) && (put_pixel_img(fdf, p1, p1.border));
	if (d.x > d.y)
		put_hor(fdf, p1, p2, d);
	else
		put_ver(fdf, p1, p2, d);
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
	if (z > 0)
		p.color = fdf->color2;
	else
		p.color = fdf->color1;
	// p.x = fdf->xoffset + (p.x - p.y) * cos(0.523599);
	// p.y = fdf->yoffset + (p.x + p.y) * sin(0.523599) - z;
	p.x = fdf->xoffset + (cte1 * p.x - cte2 * p.y);
	p.y = fdf->yoffset + (-z + (cte1 / 2.0) * p.x + (cte2 / 2.0) * p.y);
	p.border = 0;
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
	if (z > 0)
		p.color = fdf->color2;
	else
		p.color = fdf->color1;
	p.x = p.x + cte * z;
	p.y = p.y + (cte / 2.0) * z;
	p.border = 0;
	return (p);
}

void		put_legend(t_fdf *fdf)
{
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 10, ROYALBLUE,
	"1 or +           - increase altitude");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 50, ROYALBLUE,
	"2 or -           - decrease altitude");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 90, ROYALBLUE,
	"scroll up        - zoom");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 130, ROYALBLUE,
	"scroll down      - decrease altitude");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 170, ROYALBLUE,
	"Q  or space      - change projection");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 210, ROYALBLUE,
	"left arrow or A  - move left");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 250, ROYALBLUE,
	"right arrow or D - move right");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 290, ROYALBLUE,
	"up arrow or W    - move up");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 330, ROYALBLUE,
	"down arrow or S  - move down");
}

void		put_borders(t_fdf *fdf)
{
	put_line(fdf, (t_point){.x = 4, .y = 1300, .color = ROYALBLUE, .border = 1},
					(t_point){.x = 2000, .y = 1300, .color = ROYALBLUE, .border = 1});
	put_line(fdf, (t_point){.x = 2000, .y = 1300, .color = ROYALBLUE, .border = 1}
						, (t_point){.x = 2000, .y = 4, .color = ROYALBLUE, .border = 1});
	put_line(fdf, (t_point){.x = 4, .y = 1300, .color = ROYALBLUE, .border = 1},
					(t_point){.x = 4, .y = 4, .color = ROYALBLUE, .border = 1});
	put_line(fdf, (t_point){.x = 4, .y = 4, .color = ROYALBLUE, .border = 1},
					(t_point){.x = 2000, .y = 4, .color = ROYALBLUE, .border = 1});
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
			put_line(fdf, p1, p2);
			p2 = (t_point){.x = i, .y = j == fdf->map->y - 1 ? j : j + 1};
			p2 = fdf->rasterise(fdf, p2, fdf->map->board[p2.y][p2.x]);
			put_line(fdf, p1, p2);
			i++;
		}
	put_borders(fdf);
		draw_circle(fdf, 100, (t_point){.x = 2300, .y = 700});
}

void		center_x(t_fdf *fdf)
{
	t_point p1;
	t_point p2;

	p1 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1, .y = 0},
															fdf->map->board[0][fdf->map->x - 1]);
	p2 = fdf->rasterise(fdf, (t_point){.x = 0, .y = fdf->map->y - 1},
										fdf->map->board[fdf->map->y - 1][0]);
	while (p2.x < 4 || (DRAW_WIDTH - p1.x > p2.x - 4))
	{
		fdf->xoffset += 10;
		p1 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1, .y = 0},
																fdf->map->board[0][fdf->map->x - 1]);
		p2 = fdf->rasterise(fdf, (t_point){.x = 0, .y = fdf->map->y - 1},
											fdf->map->board[fdf->map->y - 1][0]);
	}
	while (p1.x > DRAW_WIDTH || (DRAW_WIDTH - p1.x < p2.x - 4))
	{
		fdf->xoffset -= 10;
		p1 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1, .y = 0},
																fdf->map->board[0][fdf->map->x - 1]);
		p2 = fdf->rasterise(fdf, (t_point){.x = 0, .y = fdf->map->y - 1},
											fdf->map->board[fdf->map->y - 1][0]);
	}
}

void		draw_circle(t_fdf *fdf, int rayon, t_point center)
{
	float angle;
	int i;
	int save;
	int colors[8] = {YELLOW, RED, ROYALBLUE, GREEN3, WHITE, BROWN, PURPLE, PINK};
	int j;
	int radius = 180;

	i = 1;
	j = 0;
	rayon = 0;
	angle = 0;
	save = angle;
	(void)colors;
	for(int y=-radius; y<=radius; y++)
	{
		// i++;
		// if (!(i % 45))
		// {
		// 	j++;
		// }
    for(int x=-radius; x<=radius; x++)
		{
			i++;
        if(x*x+y*y <= radius*radius)
  put_pixel_img(fdf, (t_point){.x = center.x + x, .y = center.y + y, .color = colors[j], .border = 1}, 1);
	if (!(i % 16290))
		j++;
	}
}
	// while (angle <= 360)
	// {
	// 	i = 0;
	// 	if (save != (int)angle)
	// 	{
	// 		save = angle;
	// 		if (!(save % 45))
	// 		{
	// 			j++;
	// 		}
	// 	}
	// 	while (i < 100)
	// 	{
	// 		// printf("color: %d\n", colors[j]);
	// 		mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, center.x + i * cos(angle), center.y + i * sin(angle), colors[j]);
	//
	// 		// put_pixel_img(fdf, (t_point){.x = center.x + i * cos(angle), .y = center.y + i * sin(angle), .color = colors[j], .border = 1}, 1);
	// 		i++;
	// 	}
	// 	angle += (2.0 * M_PI / 800.0);
	// }
}

void		center_y(t_fdf *fdf)
{
	t_point p1;
	t_point p2;

	p1 = fdf->rasterise(fdf, (t_point){.x = 0, .y = 0}, fdf->map->board[0][0]);
	p2 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1,
		.y = fdf->map->y - 1}, fdf->map->board[fdf->map->y - 1][fdf->map->x - 1]);
	while (p1.y < 4 || (DRAW_HEIGHT - p2.y > p1.y - 4))
	{
		fdf->yoffset += 10;
		p1 = fdf->rasterise(fdf, (t_point){.x = 0, .y = 0}, fdf->map->board[0][0]);
		p2 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1,
			.y = fdf->map->y - 1}, fdf->map->board[fdf->map->y - 1][fdf->map->x - 1]);
	}

	while (p2.y > DRAW_HEIGHT || (DRAW_HEIGHT - p2.y < p1.y - 4))
	{
		fdf->yoffset -= 10;
		p1 = fdf->rasterise(fdf, (t_point){.x = 0, .y = 0}, fdf->map->board[0][0]);
		p2 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1,
			.y = fdf->map->y - 1}, fdf->map->board[fdf->map->y - 1][fdf->map->x - 1]);
	}
}

void		center(t_fdf *fdf)
{
	center_y(fdf);
	center_x(fdf);
}

void		scale_zoom(t_fdf *fdf)
{
	int zoom;

	zoom = 1;
	while (fdf->map->x * zoom < DRAW_WIDTH - 10)
		zoom++;
	fdf->zoom = zoom - (zoom / 5);
}

int				handle_mouse(int button, int x, int y, t_fdf *fdf)
{
	if (x > 4 && x < DRAW_WIDTH && y > 4 && y < DRAW_HEIGHT)
	{
		fdf->zoom = button == SCROLLUP ? fdf->zoom + 1 : fdf->zoom;
		fdf->zoom = button == SCROLLDOWN ? fdf->zoom - 1 : fdf->zoom;
		process(fdf);
	}
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
	fdf->xoffset = 450;
	fdf->yoffset = 250;
	mlx_key_hook(fdf->win_ptr, &handle_key, fdf);
	mlx_mouse_hook(fdf->win_ptr, &handle_mouse, fdf);
	fdf->altitude = 3;
	fdf->rasterise = &rasterise_iso;
	fdf->img = NULL;
	fdf->color1 = WHITE;
	fdf->color2 = PURPLE;
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
