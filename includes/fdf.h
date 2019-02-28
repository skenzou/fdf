/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 00:03:00 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/28 15:11:20 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/includes/libft.h"
# include <stdio.h>
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# include "colors.h"
# define WIN_WIDTH 2500
# define WIN_HEIGHT 1500
# define DRAW_WIDTH 2000
# define DRAW_HEIGHT 1300
# define ESCAPE 53
# define UPARROW 126
# define DOWNARROW 125
# define RIGHTARROW 124
# define LEFTARROW 123
# define PLUS 69
# define MINUS 78
# define AKEY 0
# define SKEY 1
# define DKEY 2
# define QKEY 12
# define WKEY 13
# define SCROLLUP 5
# define SCROLLDOWN 4
# define SPACE 49
# define RKEY 15
# define PALETTERADIUS 100

typedef struct		s_map
{
	int				**board;
	int				x;
	int				y;
}					t_map;

typedef struct		s_point
{
	int				x;
	int				y;
	int				color;
	int				border;
}					t_point;

typedef struct		s_image
{
	void		*ptr;
	char		*data;
	int			bpp;
	int			sizeline;
	int			endian;
}					t_image;

typedef struct		s_fdf
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_image		*img;
	t_map		*map;
	int			altitude;
	int			zoom;
	int			xoffset;
	int			yoffset;
	int			color1;
	int			color2;
	t_list		*colors;
	int			colorslist;
	int			spectrum[12];
	t_point		(*rasterise)(struct s_fdf *fdf, t_point point, int z);
}					t_fdf;

t_point				rasterise_par(t_fdf *fdf, t_point point, int z);
t_point				rasterise_iso(t_fdf *fdf, t_point point, int z);
void				put_legend(t_fdf *fdf);
void				put_borders(t_fdf *fdf);
void				draw(t_fdf *fdf);

int					handle_key(int keycode, t_fdf *fdf);
int					handle_mouse(int button, int x, int y, t_fdf *fdf);
void				fill_spectrum(t_fdf *fdf);

int					put_pixel_img(t_fdf *fdf, t_point p, int border);
void				process(t_fdf *fdf);

t_map				*create_map(int fd);

void				center(t_fdf *fdf);
void				scale_zoom(t_fdf *fdf);

void				put_line(t_fdf *fdf, t_point p1, t_point p2);
void				draw_color_spectrum(t_fdf *fdf);

#endif
