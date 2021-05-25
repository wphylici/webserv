/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wphylici <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 00:35:14 by wphylici          #+#    #+#             */
/*   Updated: 2020/08/12 09:29:00 by wphylici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int		my_free(char **s, int where_free, int ret_value)
{
	int i;

	i = FD_MAX;
	if (where_free < 0)
	{
		while (i)
		{
			if (s[i])
			{
				free(s[i]);
				s[i] = NULL;
			}
			i--;
		}
	}
	else if (s[where_free])
	{
		free(s[where_free]);
		s[where_free] = NULL;
	}
	return (ret_value);
}

int		writing_in_line(char **line, char *buf)
{
	char *tmp;

	tmp = *line;
	if (!(*line = ft_strjoin(tmp, buf)))
		return (ERROR);
	my_free(&tmp, 0, 0);
	return (0);
}

int		check_remainder(char **line, char *remainder)
{
	char	*pointer_to_n_2;

	pointer_to_n_2 = NULL;
	if (remainder)
		if ((pointer_to_n_2 = ft_strchr(remainder, '\n')))
		{
			*pointer_to_n_2 = '\0';
			if (!(*line = ft_strdup(remainder)))
				return (ERROR);
			ft_strcpy(remainder, ++pointer_to_n_2);
			return (1);
		}
		else
		{
			if (!(*line = ft_strdup(remainder)))
				return (ERROR);
		}
	else
	{
		if (!(*line = ft_strdup("")))
			return (ERROR);
	}
	return (0);
}

int		get_line(int fd, char **line, char **remainder, int check)
{
	char		*buf;
	char		*pointer_to_n;
	int			how_many_was_read;

	pointer_to_n = NULL;
	how_many_was_read = 0;
	if (!(buf = (char *)malloc(B_S + 1)))
		return (ERROR);
	if ((read(fd, buf, 0)) < 0)
		return (my_free(&buf, 0, -1));
	while (!pointer_to_n && !check && (how_many_was_read = read(fd, buf, B_S)))
	{
		buf[how_many_was_read] = '\0';
		if ((pointer_to_n = ft_strchr(buf, '\n')))
		{
			*pointer_to_n = '\0';
			if (!(remainder[fd] = ft_strdup(++pointer_to_n)))
				return (my_free(&buf, 0, -1));
		}
		if (writing_in_line(line, buf) == -1)
			return (my_free(&buf, 0, -1));
	}
	my_free(&buf, 0, 0);
	return ((how_many_was_read || check) ? READ : EOF);
}

int		get_next_line(int fd, char **line)
{
	int			res;
	int			check;
	static char *remainder[FD_MAX];

	res = 0;
	check = 0;
	if (!line || fd < 0 || fd > FD_MAX || B_S < 1 || B_S > 2147483647)
		return (my_free(remainder, -1, -1));
	if ((check = check_remainder(line, remainder[fd])) == -1)
		return (my_free(remainder, -1, -1));
	else if (check != 1)
		my_free(remainder, fd, 0);
	if ((res = get_line(fd, line, remainder, check)) == -1)
		return (my_free(remainder, -1, -1));
	return (res);
}
