/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 00:03:00 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/21 11:13:04 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/includes/libft.h"
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080

typedef struct	s_map
{
	int				**board;
	int				x;
	int				y;
}				t_map;

typedef struct	s_point
{
	int				x;
	int				y;
}				t_point;

typedef struct	s_image
{
	void		*ptr;
	char		*data;
	int			bpp;
	int			sizeline;
	int			endian;
}				t_image;

typedef struct	s_fdf
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_image		*img;
	t_map		*map;
	int			altitude;
	int			zoom;
	int			xoffset;
	int			yoffset;
}				t_fdf;

t_map			*create_map(int fd);
void			draw(t_fdf *fdf);
void	create_image(t_fdf *fdf);

#endif
