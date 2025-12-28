*This project has been created as part of the 42 curriculum by icezar-s*

# get\_next\_line

## Description

The get\_next\_line project entails concepts such as memory management, low-level I/O functions and algorithm implementation with C language. 

The goal of this project is to be able to develop a program that opens and reads text files, by extracting of them one line of text at a time. 

For the bonus part, my program must be able to deal with multiple file descriptors, subsequently reading a line from each file at a time - one line of a file A, then one line of a file B, etc..

## Instructions

This project is passable of testing and use after a programmer write a proper C program calling the function get\_next\_line, feeding it a file descriptor.

One can clone this repo, and at the root of the project, create a C file that includes the get\_next\_line header. Therefore, the function will be useful in said file context. Alternatively, the programmer can compile the files in this repo as a C library - whatever suits them.

For a simple demo, you can copy the following and save it as a C file. For it to function, one must have a text file called "test.txt" on the root folder as well, which you can create in a myriad of ways that this README will not cover. 

```
#include <stdio.h>
#include "get_next_line.h"

int	main()
{
	int	fd;
	char	*line;

	fd = open("./test.txt", O_RDONLY, 0);
	while ((line = get_next_line(fd))
		printf("%s", line);
	close(fd);
	return (0);
}
```

You will find this very program, alongside with the "test.txt" file in this repo, as a courtesy. The text file contains a preem piece of art.

Assuming you are using a bash terminal, you can compile this program with the following command line:

```
cc -Wall -Wextra -Werror demo.c get_next_line.c get_next_line_utils.c -o test
```

And then you can run it by subsequently using this command line:

```
./demo
```

## How it Works - Mandatory Part

Let's examine the get\_next\_line function, while doing a deep dive at the functions it calls. 

These functions are defined at a file called `get_next_line_utils.c`, but all the relevant extracts will be at display in this README.

```
char	*get_next_line(int fd)
{
	static	char buf[BUFFER_SIZE];
	char	*line;
	char	*newline;
	int	n;

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
```

### "ft\_memchr" and "BUFFER\_SIZE"

```
newline = (char *)ft_memchr(buf, '\n', BUFFER_SIZE);
```

##### "BUFFER\_SIZE"

The first thing that needs to be understood is how we can define and read a line.

A line is everything that ends with a "\n", a character that represents a line break in the ASCII table.Other than that, a line is everything that is between a last "\n" and the proper end of the file (EOF).

But the computer can't actually discern this definition natively. He knows what the EOF is and we can show it where a "\n" can be found, but it doesn't know precisely how many bytes of information it must access until it finds the end of a line. 

Actually, that would be difficult as a human as well - we usually search for the end of a line by jumping chunks of words and text while reading. If we feel we have jumped after a line has ended, we then search for a period or a end of paragraph within the chunk we jumped over; and if we can see that the line hasn't ended yet, we jump for the next chunk.

That's exactly what we can tell the computer to do.

`BUFFER_SIZE` refers to the width of the chunk we are jumping. The larger it is, the more information we can work with by chunk. The unit with which it works with is bytes. Since an unsigned character type is represented by exactly 8 bits of information - a byte - the number associated with the `BUFFER_SIZE` is directly associated with the number of characters the program reads at a time.

As we can see, `BUFFER_SIZE` also determines the size of the memory allocated to temporarily store the characters read, so we can analyze the string formed by them in search of the end of line. So, for a larger `BUFFER_SIZE`, we'll have to have a larger free memory space.
