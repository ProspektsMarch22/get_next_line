*This project has been created as part of the 42 curriculum by icezar-s*

# get\_next\_line

## Description

The get\_next\_line project entails concepts such as memory management, low-level I/O functions and algorithm implementation with C language. 

The goal of this project is to be able to develop a program that opens and reads text files, by extracting of them one line of text at a time. 

For the bonus part, my program must be able to deal with multiple files at a time, subsequently reading a line from each file at a time - one line of a file A, then one line of a file B, etc..

## Instructions

This project is passable of testing and use after a programmer write a proper C program calling the function get\_next\_line, feeding it a file descriptor.

One can clone this repo, and at the root of the project, create a C file that includes the get\_next\_line header. Therefore, the function will be useful in said file context. Alternatively, the programmer can compile the files in this repo as a C library - that is up to them.

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

You will find this very program, alongside with the "test.txt" file in this repo, as a courtesy.

Assuming you are using a bash terminal, you can compile this program with the following command line:

```
cc -Wall -Wextra -Wextra demo.c get_next_line.c get_next_line_utils.c -o test
```

And then you can run it by subsequently using this command line:

```
./demo
```
