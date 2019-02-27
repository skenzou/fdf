/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/27 04:32:09 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_fdf	*init_fdf(int fd)
{
	t_fdf	*fdf;

	if (!(fdf = (t_fdf *)malloc(sizeof(t_fdf))) || fd == -1)
	{
		perror("error");
		exit(1);
	}
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
	fill_spectrum(fdf);
	scale_zoom(fdf);
	center(fdf);
	return (fdf);
}

int				main(int argc, char **argv)
{
	t_fdf	*fdf;

	argc != 2 ? exit(1) : 0;
	fdf = init_fdf(open(argv[1], O_RDONLY));
	process(fdf);
	mlx_loop(fdf->mlx_ptr);
	return (0);
}
