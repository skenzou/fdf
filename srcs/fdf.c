/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 23:59:47 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/20 15:24:22 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>

void	print_map(t_map *map)
{
	int i;
	int j;

	j = 0;
	while (j < map->y)
	{
		i = 0;
		while (i < map->x)
		{
			printf("%*d", !i ? 0 : 3, map->board[j][i]);
			i++;
		}
		printf("\n");
		j++;
	}
}

int		handle_key(int keycode, void *param)
{
	(void)param;
	if (keycode == 53)
		exit(0);
	printf("You pressed the key number: %d\n", keycode);
	return (1);
}

// void		put_line(t_fdf *fdf, int x1, int y1, int x2, int y2)
// {
// 	int dx;
// 	int dy;
// 	int e;
// 	e = x2 - x1;
// 	dx = e * 2;
// 	dy = (y2 - y1) * 2;
// 	while (x1 <= x2)
// 	{
// 		mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, x1 , y1, 0xFF0000);
// 		x1++;
// 		if ((e -= dy) <= 0)
// 		{
// 			y1++;
// 			e += dx;
// 		}
// 	}
// }

// procédure tracerSegment(entier x1, entier y1, entier x2, entier y2) est
//   déclarer entier dx, dy ;
//   déclarer entier e ; // valeur d’erreur
//   e  ← x2 - x1 ;        // -e(0,1)
//   dx ← e × 2 ;          // -e(0,1)
//   dy ← (y2 - y1) × 2 ;  // e(1,0)
//   tant que x1 ≤ x2 faire
//     tracerPixel(x1, y1) ;
//     x1 ← x1 + 1 ;  // colonne du pixel suivant
//     si (e ← e - dy) ≤ 0 alors  // erreur pour le pixel suivant de même rangée
//       y1 ← y1 + 1 ;  // choisir plutôt le pixel suivant dans la rangée supérieure
//       e ← e + dx ;  // ajuste l’erreur commise dans cette nouvelle rangée
//     fin si ;
//   fin faire ;
//   // Le pixel final (x2, y2) n’est pas tracé.
// fin procédure ;
void    put_line(t_fdf *fdf, int x1, int y1, int x2,int y2)
{
    int		i;
    int		dx;
	int		dy;
    int		var;

	x2 = x2 - x1;
	y2 = y2 - y1;
	dx = x2 > 0 ? 1 : -1;
	dy = y2 > 0 ? 1 : -1;
	x2 = ABS(x2);
	y2 = ABS(y2);
    mlx_pixel_put(fdf->mlx_ptr,fdf->win_ptr, x1, y1, 0xFF0000) && (i = 1);
    var = (x2 > y2 ? x2 : y2) / 2;
    if (x2 > y2)
        while (i++ <= x2 && (x1 += dx) && (var += y2))
        {
			(var >= x2) && (y1 += dy);
            (var >= x2) && (var -= x2);
            mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, x1, y1, 0xFF0000);
        }
    else
        while (i++ <= y2 && (y1 += dy) && (var += x2))
        {
            (var >= y2) && (x1 += dx);
            (var >= y2) && (var -= y2);
            mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, x1, y1, 0xFF0000);
        }
}
// void    ligne(t_mlx m, t_point o, t_point f, int color)
// {
//     int        i;
//     t_point    inc;
//     int        var;
//
//     f = (t_point){.x = f.x - o.x, .y = f.y - o.y};
//     inc = (t_point){.x = (f.x > 0) ? 1 : -1, .y = (f.y > 0) ? 1 : -1};
//     f = (t_point){.x = abs(f.x), .y = abs(f.y)};
//     p(m, o, color) && (i = 1);
//     var = ft_round((f.x > f.y ? f.x : f.y) / 2, 0);
//     if (f.x > f.y)
//         while (i++ <= f.x && (o.x += inc.x) && (var += f.y))
//         {
//             (var >= f.x) && (var -= f.x);
//             (var >= f.x) && (o.y += inc.y);
//             p(m, o, color);
//         }
//     else
//         while (i++ <= f.y && (o.y += inc.y) && (var += f.x))
//         {
//             (var >= f.y) && (o.x += inc.x);
//             (var >= f.y) && (var -= f.y);
//             p(m, o, color);
//         }
// }

int		main(int argc, char **argv)
{
	t_fdf	*fdf;
	int i;
	int j;
	// int x;
	// int y;
	int ioffset;
	int joffset;
	int z;
	double cte1;
	double cte2;
	int x2;
	int y2;
	int x;
	int y;
	//int z;

	(void)argc;
	fdf = (t_fdf *)malloc(sizeof(t_fdf));
	fdf->mlx_ptr = mlx_init();
	fdf->win_ptr = mlx_new_window(fdf->mlx_ptr, 5000, 5000, "fdf");
	fdf->map = create_map(open(argv[1], O_RDONLY));
	print_map(fdf->map);
	joffset = 20;
	ioffset = 20;
	mlx_key_hook(fdf->win_ptr, &handle_key, fdf);
	//put_line(fdf, 0 + 250, 0 + 250, -100 + 250, 100 + 250);
	// put_line(fdf, 0 + 250, 0 + 250, 10 + 250, 10 + 250);
	cte1 = 0.5;
	cte2 = 0.7;
	j = 0;
	while (j < fdf->map->y)
	{
		i = 0;
		x = 950 + (i * 20);
		y = 200 + (j * 20);
		z = fdf->map->board[j][i];

		x = 950 + (cte1 * x - cte2 * y);
		y = 200 + (-z + (cte1 / 2.0) * x + (cte2 / 2.0) * y);
		i++;
		while (i < fdf->map->x)
		{
			x2 = 950 + (i * 20);
			y2 = 200 + (j * 20);
			z = fdf->map->board[j][i];

			x2 = 950 + (cte1 * x2 - cte2 * y2);
			y2 = 200 + (-z + (cte1 / 2.0) * x2 + (cte2 / 2.0) * y2);
			put_line(fdf, x, y, x2, y2);
			x = x2;
			y = y2;
			i++;
		}
		j++;
	}
	i = 0;
	while (i < fdf->map->x)
	{
		j = 0;
		x = 950 + (i * 20);
		y = 200 + (j * 20);
		z = fdf->map->board[j][i];

		x = 950 + (cte1 * x - cte2 * y);
		y = 200 + (-z + (cte1 / 2.0) * x + (cte2 / 2.0) * y);

		j++;
		while (j < fdf->map->y)
		{
			x2 = 950 + (i * 20);
			y2 = 200 + (j * 20);
			z = fdf->map->board[j][i];

			x2 = 950 + (cte1 * x2 - cte2 * y2);
			y2 = 200 + (-z + (cte1 / 2.0) * x2 + (cte2 / 2.0) * y2);
			put_line(fdf, x, y, x2, y2);
			x = x2;
			y = y2;
			j++;
		}
		i++;
	}
	// while (j < fdf->map->y)
	// {
	// 	i = 0;
	// 	ioffset = 250;
	// 	z = fdf->map->board[j][i];
	// 	x = ioffset + i + cte * z;
	// 	y = joffset + j + (cte / 2.0) * z;
	// 	i++;
	// 	while (i < fdf->map->x)
	// 	{
	// 		z = fdf->map->board[j][i];
	// 		put_line(fdf,x , y, ioffset + i + cte * z, joffset + j + (cte / 2.0) * z);
	// 		x = ioffset + i + cte * z;
	// 		y = joffset + j + (cte / 2.0) * z;
	// 		i++;
	// 		ioffset += 20;
	// 	}
	// 	joffset += 20;
	// 	j++;
	// }
	// i = 0;
	// ioffset = 250;
	// while (i < fdf->map->x)
	// {
	// 	j = 0;
	// 	joffset = 250;
	// 	z = fdf->map->board[j][i];
	// 	x = ioffset + i + cte * z;
	// 	y = joffset + j + (cte / 2.0) * z;
	// 	j++;
	// 	while (j < fdf->map->y)
	// 	{
	// 		z = fdf->map->board[j][i];
	// 		put_line(fdf,x , y, ioffset + i + cte * z, joffset + j + (cte / 2.0) * z);
	// 		x = ioffset + i + cte * z;
	// 		y = joffset + j + (cte / 2.0) * z;
	// 		j++;
	// 		joffset += 20;
	// 	}
	// 	ioffset += 20;
	// 	i++;
	// }
	mlx_loop(fdf->mlx_ptr);
	return (0);
}

// -------------------------------------------------------------------------
// void	print_point(t_list *list)
// {
// 	int			i;
// 	t_point		*point;
//
// 	i = 0;
// 	while (list)
// 	{
// 		point = (t_point*)list->content;
// 		printf("Le point numero %d x: %d y: %d z: %d\n", i++, point->x,
// 			point->y, point->z);
// 		list = list->next;
// 	}
// }

// t_point		*create_point(int x, int y, int z, int fd)
// {
// 	t_point *point;
//
// 	if (!(point = (t_point *)malloc(sizeof(t_point))))
// 	{
// 		close(fd);
// 		exit(1);
// 	}
// 	point->x = x;
// 	point->y = y;
// 	point->z = z;
// 	return (point);
// }

// // static	t_list	*line_to_points(str,)
// int		create_list(int fd, int *x, int *y, t_list **begin)
// {
// 	char		*str;
// 	char		*save;
// 	int			ret;
// 	t_list		*lst;
//
// 	while ((ret = get_next_line(fd, &str)) > 0)
// 	{
// 		*x = 0;
// 		save = str;
// 		while (str && *str)
// 		{
// 			if (ft_isdigit(*str) || *str == '-')
// 			{
// 				if (!(lst = ft_lstnew(create_point(
// 							(*x)++, *y, ft_atoi(str), fd), sizeof(t_point))))
// 					return (ft_lstdestroy(begin));
// 				ft_lstadd(begin, lst);
// 				while (ft_isdigit(*(str + 1)))
// 					str++;
// 			}
// 			str++;
// 		}
// 		ft_strdel(&save);
// 		(*y)++;
// 	}
// 	ret ? ft_lstdestroy(begin) : ft_lstrev(begin);
// 	return (1);
// }
//
// // static	void	destroy_map(int **map, int x, int y)
// // {
// //
// // }
// static	int		**create_map(int fd)
// {
// 	t_list	*list;
// 	int		**map;
// 	int		*tab;
// 	t_point *point;
// 	int		x;
// 	int		y;
// 	int		i;
// 	int		j;
//
// 	y = 0;
// 	j = 0;
// 	list = NULL;
// 	create_list(fd, &x, &y, &list);
// 	map = (int **)malloc(sizeof(int *) * y);
// 	while (map && j < y)
// 	{
// 		i = 0;
// 		tab = (int *)malloc(sizeof(int) * x);
// 		while (list && i < x && (point = (t_point*)list->content))
// 		{
// 			tab[i++] = point->z;
// 			list = list->next;
// 		}
// 		map[j++] = tab;
// 	}
// 	//print_map(map, x, y);
// 	return (map);
// }
