/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 03:47:26 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/27 04:32:07 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int		compare_points(t_point p1, t_point p2)
{
	return (p1.x == p2.x && p1.y == p2.y);
}

static int		fetch_color(t_fdf *fdf, t_point p)
{
	t_list *list;

	list = fdf->colors;
	while (list)
	{
		if (compare_points(*(t_point *)(list->content), p))
			return (((t_point *)list->content)->color);
		list = list->next;
	}
	return (0);
}

int				handle_mouse(int button, int x, int y, t_fdf *fdf)
{
	if (x > 4 && x < DRAW_WIDTH && y > 4 && y < DRAW_HEIGHT)
	{
		fdf->zoom = button == SCROLLUP ? fdf->zoom + 1 : fdf->zoom;
		fdf->zoom = button == SCROLLDOWN ? fdf->zoom - 1 : fdf->zoom;
	}
	if (x >= 2200 && x <= 2400 && y <= 800 && y >= 600)
	{
		if (button == 1)
			fdf->color1 = fetch_color(fdf, (t_point){.x = x, .y = y});
		if (button == 2)
			fdf->color2 = fetch_color(fdf, (t_point){.x = x, .y = y});
	}
	process(fdf);
	return (1);
}

int				handle_key(int keycode, t_fdf *fdf)
{
	keycode == ESCAPE ? exit(0) : 0;
	(keycode == RIGHTARROW || keycode == DKEY) && (fdf->xoffset += 30);
	(keycode == LEFTARROW || keycode == AKEY) && (fdf->xoffset -= 30);
	(keycode == UPARROW || keycode == WKEY) && (fdf->yoffset -= 30);
	(keycode == DOWNARROW || keycode == SKEY) && (fdf->yoffset += 30);
	(keycode == PLUS || keycode == 18) && (fdf->altitude++);
	(keycode == MINUS || keycode == 19) && (fdf->altitude--);
	(keycode == RKEY) && (fdf->xoffset = 450);
	(keycode == RKEY) && (fdf->yoffset = 250);
	(keycode == RKEY) && (fdf->altitude = 3);
	keycode == RKEY ? scale_zoom(fdf) : 0;
	keycode == RKEY && fdf->rasterise == rasterise_par ? fdf->zoom /= 2 : 0;
	keycode == RKEY ? center(fdf) : 0;
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

void			fill_spectrum(t_fdf *fdf)
{
	fdf->spectrum[0] = BLUE;
	fdf->spectrum[1] = LIGHTBLUE;
	fdf->spectrum[2] = TURQUOISE;
	fdf->spectrum[3] = GREENBLUE;
	fdf->spectrum[4] = GREEN;
	fdf->spectrum[5] = LIGHTGREEN;
	fdf->spectrum[6] = YELLOW;
	fdf->spectrum[7] = ORANGE;
	fdf->spectrum[8] = RED;
	fdf->spectrum[9] = LIGHTRED;
	fdf->spectrum[10] = MAGENTA;
	fdf->spectrum[11] = VIOLET;
}
