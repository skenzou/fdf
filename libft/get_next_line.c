/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 08:54:44 by midrissi          #+#    #+#             */
/*   Updated: 2018/11/27 12:15:04 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int		get_line_length(char *s)
{
	int i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	return (i);
}

static	char	*read_line(char *s, int fd, int *ret)
{
	char	buffer[BUFF_SIZE + 1];
	char	*tmp;

	while ((*ret = read(fd, buffer, BUFF_SIZE)) > 0)
	{
		buffer[*ret] = '\0';
		tmp = s;
		if (!(s = ft_strjoin(s, buffer)))
		{
			ft_strdel(&tmp);
			*ret = -1;
			return (NULL);
		}
		ft_strdel(&tmp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (s);
}

int				get_next_line(const int fd, char **line)
{
	static char	*s[FD_MAX + 1];
	int			ret;
	int			len;

	if (!line || fd < 0 || fd >= FD_MAX || (!s[fd] && !(s[fd] = ft_strnew(0))))
		return (-1);
	s[fd] = read_line(s[fd], fd, &ret);
	if (s[fd] && ft_strlen(s[fd]))
	{
		len = get_line_length(s[fd]);
		if (!(*line = ft_strsub(s[fd], 0, len)))
			ft_strdel(&s[fd]);
		s[FD_MAX] = s[fd];
		s[fd] = ft_strsub(s[fd], len + 1, ft_strlen(s[fd]) - len);
		ft_strdel(&s[FD_MAX]);
		return (!s[fd] ? -1 : 1);
	}
	ft_strdel(&s[fd]);
	return (!ret ? 0 : -1);
}
