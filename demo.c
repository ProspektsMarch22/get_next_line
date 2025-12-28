#include <stdio.h>
#include "get_next_line.h"

int	main()
{
	int	fd;
	char	*line;

	fd = open("./test.txt", O_RDONLY, 0);
	while ((line = get_next_line(fd)))
		printf("%s", line);
	close(fd);
	return(0);
}
