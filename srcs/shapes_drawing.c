/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes_drawing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 03:52:05 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/28 15:13:07 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		put_hor(t_fdf *fdf, t_point p1, t_point p2, t_point d)
{
	int		j;
	int		var;
	t_point i;
	t_point curr;

	curr = (t_point){.x = p1.x, .y = p1.y, .color = p1.color};
	i = (t_point){.x = p2.x - p1.x > 0 ? 1 : -1, .y = p2.y - p1.y > 0 ? 1 : -1};
	j = 1;
	var = (d.x > d.y ? d.x : d.y) / 2;
	while (j++ <= d.x && (var += d.y))
	{
		curr.x += i.x;
		(var >= d.x) && (curr.y += i.y);
		(var >= d.x) && (var -= d.x);
		(curr.color = ft_get_color(p1.color, p2.color, d.x > d.y ?
		ft_percent(p1.x, p2.x, curr.x) : ft_percent(p1.y, p2.y, curr.y))) &&
										(put_pixel_img(fdf, curr, p1.border));
	}
}

static void		put_ver(t_fdf *fdf, t_point p1, t_point p2, t_point d)
{
	int		j;
	int		var;
	t_point i;
	t_point curr;

	curr = (t_point){.x = p1.x, .y = p1.y, .color = p1.color};
	i = (t_point){.x = p2.x - p1.x > 0 ? 1 : -1, .y = p2.y - p1.y > 0 ? 1 : -1};
	j = 1;
	var = (d.x > d.y ? d.x : d.y) / 2;
	while (j++ <= d.y && (var += d.x))
	{
		curr.y += i.y;
		(var >= d.y) && (curr.x += i.x);
		(var >= d.y) && (var -= d.y);
		(curr.color = ft_get_color(p1.color, p2.color, d.x > d.y ?
		ft_percent(p1.x, p2.x, curr.x) : ft_percent(p1.y, p2.y, curr.y))) &&
										(put_pixel_img(fdf, curr, p1.border));
	}
}

void			put_line(t_fdf *fdf, t_point p1, t_point p2)
{
	t_point d;

	d = (t_point){.x = p2.x - p1.x, .y = p2.y - p1.y};
	d = (t_point){.x = abs(d.x), .y = abs(d.y)};
	(p1.color = ft_get_color(p1.color, p2.color, d.x > d.y ?
		ft_percent(p1.x, p2.x, p1.x) : ft_percent(p1.y, p2.y, p1.y))) &&
											(put_pixel_img(fdf, p1, p1.border));
	if (d.x > d.y)
		put_hor(fdf, p1, p2, d);
	else
		put_ver(fdf, p1, p2, d);
}

static void		put_circle_point(t_fdf *fdf, int i, float angle, int color)
{
	t_point	point;
	t_point	c;
	t_list	*list;

	c = (t_point){.x = 2300, .y = 700};
	point = (t_point){.x = c.x + i * cos(angle), .y = c.y + i * sin(angle),
			.color = 0, .border = 1};
	point.color = ft_get_color(color, WHITE, 1 - i / (float)PALETTERADIUS);
	put_pixel_img(fdf, point, 1);
	if (!(fdf->colorslist))
	{
		if (!(list = ft_lstnew((void*)&point, sizeof(t_point))))
			exit(1);
		ft_lstadd(&fdf->colors, list);
	}
}

void			draw_color_spectrum(t_fdf *fdf)
{
	float	angle;
	float	i;
	int		j;
	float	tmp;

	j = 0;
	angle = 0;
	tmp = M_PI / 6.0;
	while (angle <= 2.0 * M_PI)
	{
		i = 0;
		if (angle > tmp && ++j)
			tmp += M_PI / 6.0;
		while (i <= (float)PALETTERADIUS)
		{
			put_circle_point(fdf, i, angle, fdf->spectrum[j]);
			i++;
		}
		angle += 0.005;
	}
	fdf->colorslist = 1;
}
