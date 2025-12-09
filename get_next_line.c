/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icezar-s <icezar-s@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 19:11:11 by icezar-s          #+#    #+#             */
/*   Updated: 2025/12/09 02:11:15 by icezar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "get_next_line.h"

static int	read_buffer(char *pbuf, char **line)
{
	size_t		i;

	i = 0;
	while(pbuf[i])
	{
		if (pbuf[i] == '\n')
		{
			*line = malloc(sizeof(char) * (i + 2));
			if (!line)
				return (-1);
			*line = strncpy(*line, pbuf, i + 1);
			return (0);
		}
		i++;
	}
	return (1);
}
char	*get_next_line(int fd)
{
	char		*line;
	char		buf[BUFFER_SIZE];
	char		*pbuf;
	static int	n;

	n = read(fd, buf, BUFFER_SIZE);
	line = NULL;
	if (n == -1)
		return (NULL);
	pbuf = buf;
	while (n > 0 && read_buffer(pbuf, &line))
	{
		n = read(fd, buf, BUFFER_SIZE);
		pbuf = buf;
	}
	return (line);
}

int	main(int argc, char *argv[])
{
	int	fd;
	char	*line;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY, 0);
		line = get_next_line(fd);
		printf("%s", line);
		free(line);
	}
	return (0);
}
