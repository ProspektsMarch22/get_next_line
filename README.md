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

`BUFFER_SIZE` refers to the width of the chunk we are jumping. The larger it is, the more information we can work with by chunk. The unit with which it works with is bytes. Since an unsigned character type is represented by exactly 8 bits of information - a byte - the number `BUFFER_SIZE` takes is directly associated with the number of characters the program reads at a time.

As we can see, `BUFFER_SIZE` also determines the size of the memory allocated to temporarily store the characters read, so we can analyze the string formed by them in search of the end of line. So, for a larger `BUFFER_SIZE`, we'll have to have a larger free memory space.

###### "ft\_memchr"

Let's emulate a first run of the function.

The `line` and `newline` variables are `NULL`. Therefore, when the condition `while(!newline)` is checked and evaluated to `true`, we then assign the variable `newline` the value of `ft_memchr` return, converted to the string type - a `char` pointer.

What are we doing here? We are taking the information present in the `buf` variable - the `BUFFER_SIZE` number of characters that were read - and searching within it for a end-of-line character, the `\n`.

The implementation of `ft_memchr` mirrors that of the C's standard library `strchr`.

```
void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t		i;
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
```

In this function, we take a memory area represented by a `void` pointer, and given its limits - the `size_t n` argument - we search for a specific character whithin it.

If we find said character, the function returns an address to it. If this address is in the middle of a string, we get the a substring that starts with the character.

If we do not find the character, or the size argument is invalid, the function returns `NULL`.

Backing up to our first emulated run of the `get_next_line` function, we can safely say that when called for the first time, `ft_memchr` will return NULL, since we didn't fed the `buf` variable yet.

### refresh\_buffer

So, we need to fill in the buffer, represented by the `buf` variable, with a chunk of text to be analyzed. We will accomplish that with the `refresh_buffer` function

```
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
```

The `refresh_buffer` function takes the `buf` variable, alongside with the file descriptor `fd` that was fed to the `get_next_line` function, and reads `BUFFER_SIZE` bytes of the file. 

The file descriptor is a unique integer value that represents a file in the UNIX system. For a deep dive in what this actually means, see the resources section of this README, in which there is a link to an article made by me that explains UNIX take on file representation.

What we need to know now is that the `read` function, a C UNIX Standard Library function, will do is read from the given file descriptor `BUFFER_SIZE` bytes and store them inside a given string variable - that will be our `buf`.

It them returns the amount of bytes succesfully read that way. If it returns `-1`, then it means that we cannot read the file at all. If it returns `0`, than it means we reached the EOF.

The `while` statement in this function garantees us that, if we managed to read less bytes than `BUFFER_SIZE`, the `buf` variable will fill its remaining space with a null-terminator character, the `'\0'`.

In a succesful read attempt, our `buf` variable has now a chunk of text, and `refresh_buffer` has returned the number of succesfully read bytes of information.

Going back to the `get_next_line` function, we have the following conditional statements:

```
n = refresh_buffer(buf, fd);
if (n == 0)
	return (line);
if (n == -1)
{
	free(line);
	return (NULL);
}
```

This lets us deal exactly with the edge cases - when we either finish reading the file, a condition we saw as definitive of a line, or we couldn't read the file at all, at which case the function terminates returning `NULL`.
