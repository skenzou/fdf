/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 04:07:28 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/28 15:13:35 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_point		rasterise_iso(t_fdf *fdf, t_point p, int z)
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
	p.x = fdf->xoffset + (cte1 * p.x - cte2 * p.y);
	p.y = fdf->yoffset + (-z + (cte1 / 2.0) * p.x + (cte2 / 2.0) * p.y);
	p.border = 0;
	return (p);
}

t_point		rasterise_par(t_fdf *fdf, t_point p, int z)
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
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 10, WHITE,
	"1 or +                - increase altitude");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 50, WHITE,
	"2 or -                - decrease altitude");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 90, WHITE,
	"scroll up             - zoom");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 130, WHITE,
	"scroll down           - decrease altitude");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 170, WHITE,
	"Q  or space           - change projection");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 210, WHITE,
	"left arrow or A       - move left");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 250, WHITE,
	"right arrow or D      - move right");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 290, WHITE,
	"up arrow or W         - move up");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 330, WHITE,
	"down arrow or S       - move down");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 370, WHITE,
	"left click on color   - low altitude color");
	mlx_string_put(fdf->mlx_ptr, fdf->win_ptr, DRAW_WIDTH + 10, 410, WHITE,
	"right click on color  - high altitude color");
}

void		put_borders(t_fdf *fdf)
{
	int i;

	i = DRAW_HEIGHT - 1;
	while (++i < WIN_HEIGHT)
		put_line(fdf, (t_point){.x = 0, .y = i, .color = CHERRY, .border = 1},
				(t_point){.x = 2000, .y = i, .color = CHERRY, .border = 1});
	i = DRAW_WIDTH - 1;
	while (++i < WIN_WIDTH)
		put_line(fdf, (t_point){.x = i, .y = WIN_HEIGHT - 1, .color = CHERRY,
			.border= 1}, (t_point){.x = i, .y = 0, .color = CHERRY,
																.border = 1});
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
	draw_color_spectrum(fdf);
}
