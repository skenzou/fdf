/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 12:38:34 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/08 22:25:01 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(intmax_t n)
{
	char *str;
	char *temp;

	str = ft_utoa_base(ABS(n), 10, 0);
	if (n < 0)
	{
		temp = str;
		str = ft_strjoin("-", str);
		ft_strdel(&temp);
	}
	return (str);
}
