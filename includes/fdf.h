/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 00:03:00 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/20 11:10:31 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/includes/libft.h"
# include <mlx.h>
# include <math.h>
# include <fcntl.h>

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
	int				z;
}				t_point;

typedef struct	s_fdf
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_map		*map;
}				t_fdf;

t_map			*create_map(int fd);

#endif
