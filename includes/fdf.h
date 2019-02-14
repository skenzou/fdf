/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 00:03:00 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/14 03:11:12 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/libft.h"
# include <math.h>
# include <fcntl.h>

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
	t_list		*list;
} 				t_fdf;

#endif
