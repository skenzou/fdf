/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 13:42:50 by midrissi          #+#    #+#             */
/*   Updated: 2018/10/15 10:36:00 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		count_strings(char const *s, char c)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i - 1] == c || i == 0))
			count++;
		i++;
	}
	return (count);
}

static size_t	get_substr_len(char const *s, int start, char c)
{
	size_t len;

	len = 0;
	while (s[start] != c && s[start])
	{
		start++;
		len++;
	}
	return (len);
}

static void		free_mem(char **strings, int len)
{
	int i;

	i = 0;
	while (i <= len)
	{
		free(strings[i++]);
	}
	free(strings);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**strings;
	int		count;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	count = count_strings(s, c);
	strings = (char **)malloc(sizeof(char *) * (count + 1));
	if (!strings)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i - 1] == c || i == 0) && j < count)
			if (!(strings[j++] = ft_strsub(s, i, get_substr_len(s, i, c))))
			{
				free_mem(strings, j - 2);
				return (NULL);
			}
		i++;
	}
	strings[j] = 0;
	return (strings);
}
