/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/28 15:11:43 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_fdf	*init_fdf(int fd)
{
	t_fdf	*fdf;

	if (!(fdf = (t_fdf *)malloc(sizeof(t_fdf))))
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
	fdf->colorslist = 0;
	fdf->colors = NULL;
	fill_spectrum(fdf);
	scale_zoom(fdf);
	center(fdf);
	return (fdf);
}

int				main(int argc, char **argv)
{
	t_fdf	*fdf;
	int		fd;

	if (argc != 2)
	{
		ft_putendl_fd("usage: fdf input_file", 2);
		return (0);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		perror("error");
	fdf = init_fdf(fd);
	process(fdf);
	mlx_loop(fdf->mlx_ptr);
	return (0);
}
