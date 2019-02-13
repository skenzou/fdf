/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/25 17:18:31 by midrissi          #+#    #+#             */
/*   Updated: 2019/02/08 22:25:03 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa_base(intmax_t n, int base, int uppercase)
{
	char *str;
	char *temp;

	str = ft_utoa_base(ABS(n), base, uppercase);
	if (n < 0)
	{
		temp = str;
		str = ft_strjoin("-", str);
		ft_strdel(&temp);
	}
	return (str);
}
