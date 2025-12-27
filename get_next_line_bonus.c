/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icezar-s <icezar-s@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:59:47 by icezar-s          #+#    #+#             */
/*   Updated: 2025/12/27 23:18:00 by icezar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#ifndef FD_MAX
# if BUFFER_SIZE > 1000
#  define FD_MAX 16
# else
#  define FD_MAX 1024
# endif
#endif

static char	*read_line(int fd, char buf[FD_MAX][BUFFER_SIZE + 1])
{
	char	*line;
	char	*newline;
	int		n;

	line = NULL;
	newline = NULL;
	while (!newline)
	{
		newline = (char *)ft_memchr(buf[fd], '\n', BUFFER_SIZE);
		if (!*buf[fd])
		{
			n = refresh_buffer(buf[fd], fd);
			if (n == 0)
				return (line);
			if (n == -1)
			{
				free(line);
				return (NULL);
			}
		}
		else
			line = arrange_buffer(buf[fd], str_shift(line, buf[fd]));
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	buf[FD_MAX][BUFFER_SIZE + 1];

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	return (read_line(fd, buf));
}
