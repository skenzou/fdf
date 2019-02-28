/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 03:50:36 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/28 14:22:22 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int				put_pixel_img(t_fdf *fdf, t_point p, int border)
{
	int offset;
	int color;

	offset = ((p.x + p.y * WIN_WIDTH) * fdf->img->bpp);
	color = mlx_get_color_value(fdf->mlx_ptr, p.color);
	if (border || (p.x < DRAW_WIDTH && p.x > 0 && p.y < DRAW_HEIGHT && p.y > 0))
		*(int *)(fdf->img->data + offset) = color;
	return (1);
}

static void		create_image(t_fdf *fdf)
{
	if (!(fdf->img = (t_image *)malloc(sizeof(t_image))))
		exit(1);
	fdf->img->ptr = mlx_new_image(fdf->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!(fdf->img->ptr))
		exit(1);
	fdf->img->data = mlx_get_data_addr(fdf->img->ptr,
			&fdf->img->bpp, &fdf->img->sizeline, &fdf->img->endian);
	fdf->img->bpp /= 8;
}

void			process(t_fdf *fdf)
{
	mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
	if (fdf->img)
		mlx_destroy_image(fdf->mlx_ptr, fdf->img->ptr);
	create_image(fdf);
	draw(fdf);
	mlx_put_image_to_window(fdf->mlx_ptr, fdf->win_ptr, fdf->img->ptr, 0, 0);
	put_legend(fdf);
}
