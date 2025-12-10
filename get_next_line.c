/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icezar-s <icezar-s@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 19:11:11 by icezar-s          #+#    #+#             */
/*   Updated: 2025/12/10 01:30:06 by icezar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE];
	char		*line;
	char		*newline;
	int			n;

	line = NULL;
	newline = NULL;
	while (!newline)
	{
		newline = (char *)ft_memchr(buf, '\n', BUFFER_SIZE);
		if (!buf[0])
		{
			n = refresh_buffer(buf, fd);
			if (n == 0)
				return (line);
			if (n == -1)
			{
				free(line);
				return (NULL);
			}
		}
		else
			line = arrange_buffer(buf, str_shift(line, buf));
	}
	return (line);
}
