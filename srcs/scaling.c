/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaling.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 03:55:44 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/28 15:16:13 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		center_x(t_fdf *fdf)
{
	t_point p1;
	t_point p2;

	p1 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1, .y = 0},
										fdf->map->board[0][fdf->map->x - 1]);
	p2 = fdf->rasterise(fdf, (t_point){.x = 0, .y = fdf->map->y - 1},
										fdf->map->board[fdf->map->y - 1][0]);
	while (p2.x < 4 || (DRAW_WIDTH - p1.x > p2.x))
	{
		fdf->xoffset += 10;
		p1 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1, .y = 0},
										fdf->map->board[0][fdf->map->x - 1]);
		p2 = fdf->rasterise(fdf, (t_point){.x = 0, .y = fdf->map->y - 1},
										fdf->map->board[fdf->map->y - 1][0]);
	}
	while (p1.x > DRAW_WIDTH || (DRAW_WIDTH - p1.x < p2.x))
	{
		fdf->xoffset -= 10;
		p1 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1, .y = 0},
										fdf->map->board[0][fdf->map->x - 1]);
		p2 = fdf->rasterise(fdf, (t_point){.x = 0, .y = fdf->map->y - 1},
										fdf->map->board[fdf->map->y - 1][0]);
	}
}

static void		center_y(t_fdf *fdf)
{
	t_point p1;
	t_point p2;

	p1 = fdf->rasterise(fdf, (t_point){.x = 0, .y = 0}, fdf->map->board[0][0]);
	p2 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1,
	.y = fdf->map->y - 1}, fdf->map->board[fdf->map->y - 1][fdf->map->x - 1]);
	while (p1.y < 4 || (DRAW_HEIGHT - p2.y > p1.y))
	{
		fdf->yoffset += 10;
		p1 = fdf->rasterise(fdf, (t_point){.x = 0, .y = 0},
														fdf->map->board[0][0]);
		p2 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1, .y =
		fdf->map->y - 1}, fdf->map->board[fdf->map->y - 1][fdf->map->x - 1]);
	}
	while (p2.y > DRAW_HEIGHT || (DRAW_HEIGHT - p2.y < p1.y))
	{
		fdf->yoffset -= 10;
		p1 = fdf->rasterise(fdf, (t_point){.x = 0, .y = 0},
														fdf->map->board[0][0]);
		p2 = fdf->rasterise(fdf, (t_point){.x = fdf->map->x - 1, .y =
		fdf->map->y - 1}, fdf->map->board[fdf->map->y - 1][fdf->map->x - 1]);
	}
}

void			center(t_fdf *fdf)
{
	center_y(fdf);
	center_x(fdf);
}

void			scale_zoom(t_fdf *fdf)
{
	int zoom;

	zoom = 1;
	while (fdf->map->x * zoom < DRAW_WIDTH - 10)
		zoom++;
	fdf->zoom = zoom - (zoom / 5);
}
