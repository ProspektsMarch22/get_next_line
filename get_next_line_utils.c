/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icezar-s <icezar-s@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 23:42:55 by icezar-s          #+#    #+#             */
/*   Updated: 2025/12/10 01:35:37 by icezar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	chr;
	unsigned char	*ptr;

	if (n == 0)
		return (NULL);
	chr = (unsigned char)c;
	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == chr)
			return ((void *)&ptr[i]);
		i++;
	}
	return (NULL);
}

static size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = 0;
	src_len = 0;
	i = 0;
	while (dst[dst_len])
		dst_len++;
	while (src[src_len])
		src_len++;
	if (size <= dst_len)
		return (size + src_len);
	while (src[i] && ((dst_len + i + 1) < size))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if ((dst_len + i) < size)
		dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

char	*str_shift(char *old_buf, char *new_buf)
{
	char	*shifted;
	size_t	old_buf_len;
	size_t	new_buf_len;

	old_buf_len = 0;
	new_buf_len = 0;
	while (old_buf && old_buf[old_buf_len])
		old_buf_len++;
	while (new_buf && new_buf[new_buf_len] && new_buf[new_buf_len] != '\n')
		new_buf_len++;
	if (new_buf && new_buf[new_buf_len] == '\n')
		new_buf_len++;
	shifted = malloc(sizeof(char) * (old_buf_len + new_buf_len + 1));
	if (!shifted)
		return (free(old_buf), NULL);
	shifted[0] = '\0';
	if (old_buf)
	{
		ft_strlcat(shifted, old_buf, old_buf_len + 1);
		free(old_buf);
	}
	ft_strlcat(shifted, new_buf, old_buf_len + new_buf_len + 1);
	return (shifted);
}

int	refresh_buffer(char *buf, int fd)
{
	int	n;
	int	i;

	n = read(fd, buf, BUFFER_SIZE);
	i = n;
	if (n == -1)
		return (n);
	while (i < BUFFER_SIZE)
	{
		buf[i] = '\0';
		i++;
	}
	return (n);
}

char	*arrange_buffer(char *buf, char *line)
{
	size_t	i;
	size_t	buf_len;

	buf_len = 0;
	i = 0;
	while (buf[buf_len] && buf[buf_len] != '\n')
		buf_len++;
	if (buf[buf_len] == '\n')
		buf_len++;
	while (buf[buf_len])
		buf[i++] = buf[buf_len++];
	while (i < BUFFER_SIZE)
		buf[i++] = '\0';
	return (line);
}
