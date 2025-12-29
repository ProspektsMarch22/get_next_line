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

##### "ft\_memchr"

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

### arrange\_buffer and str\_shift

So, lets continue our hypothetical simulation:

1. We first fed the `buf` variable with text information by going into `get_next_line`'s `if` statement, nested in the `while` condition;
2. We then go back to check if there's anything in the `newline` variable. Since our first run of the `ft_memchr` function gave us `NULL`, we still have `newline = NULL`, therefore, we go into the `while`'s body again.
3. Now, we will run `ft_memchr` again, this time with a working `buf` variable. Let's assume now we have a piece of text that has a proper line in it.
4. So we go straight to the `else` statement this time, and we then assign to the `line` variable the return value of the `arrange_buffer` function.
5. Since we got a valid return value from `ft_memchr`, after the assignment of value to `line`, we will exit the `while` statement and return the `line` variable by the end of the code.

But how we got a valid line for it to return?

Let's look at both the `arrange_buffer` and `str_shift` functions.

```
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
```

```
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
```

Following up our scenario, we have in `buf` a piece of text that contains a valid line. And what I want you to imagine is that this valid line is burrowed within all the text we have in `buf`. So we need to extract only the characters that constitute the valid line, while safely maintaining the rest of useful information currently in the `buf` that may constitute the start of another line.

Imagine now that we have a really long line, a line that has more than `BUFFER_SIZE` characters. We need to deal with a scenario where we have to `read` from the file multiple times until we have found the end-of-line character `'\n'` or the EOF itself; in other words, we need a way to keep storing more and more information until it forms a valid line.

The functions above tackle both of these scenarios.

In order of execution:
* `str_shift` effectively create a string (`shifted`) that consists of the valid line information. In the scenario where we have the valid line burrowed within the `buf`, we will concatenate the relevant piece of information to a new `shifted` variable, dynamically created with `malloc`. In the scenario where we need to read more bytes, all the characters in `buf` will then be concatenated to a new `shifted` variable each iteration, until we reach the end of line, while `free`ing the old one;
* `str_shift` will return the `shifted` variable;
* The `shifted` variable is then fed to the `arrange_buffer` function;
* The `arrange_buffer` has now a `buf` value with information that was already safely stored at the `shifted` variable. In the scenario where we had the line burrowed within the `buf` variable, we will then begin to replace the information in the `buf`, dislocating the bytes after the valid line to the beginning of the `buf`. The remaining space will be filled with the null-terminated character. In the scenario where we need to continue reading, we will just traverse the `buf`.

At the end, we return the value of the function `str_shift`, even while running the `arrange_buffer` function. This was necessary as a means to comply with the norm rules of 42 and save some lines of code.

So we did find a valid line, saved it to safe `shifted` variable that was succesfully returned and use this value to assign a new value to the `line` variable.

Since when running `ft_memchr` we returned a non-NULL value, we exit the `while` block in the `get_next_line` function, returning the value assigned to the `line` function.

If we need to read more chunks of data, `ft_memchr` would return `NULL`, so we would concatenate more information to the `shifted` variable, while cleaning the `buf` variable to receive more data, each time we iterate through the `while` loop of the `get_next_line` function.

The `str_shift` function depends on the `ft_strlcat` function, which is a safe string concatenation function mirrorring the C Standard String Library implementation of `strlcat`. A deep-dive of its inner-workings would be an overkill for the purposes of explaining this project, so we leave at that - the `str_shift` function _safely_ concatenates strings using `ft_strlcat`. If you want to, you can check up on my `ft_strlcat` implementation in both this project and at the libft project, which has its own repo as well.

### Safely storing remaining information

After safely reading the first line of the text file, imagine that in our `buf` we have the start of a new line. So, in our demo file, we call the function again, after it terminated and returned a valid line. How we can be sure that the start of the next line is safely stored?

That is why we have the `static` keyword associated with the definition of the `buf` variable. That means that the information in memory associated with that particular variable will be kept for the context of the function it belongs to, as long as the program is running.

But when we `read`, the `buf` will be updated. What then?

That's why the `if` statement nested in `get_next_line`'s `while` checks for the `buf` contents. If we have something inside `buf` that is not trash, we will call the `arrange_buffer` + `str_shift` combo on it, safely securing the information before `refresh_buffer` is called again.

## How it Works - Bonus Part

The goal of the bonus part was to make the program able to deal with multiple file descriptors at once, with variable `BUFFER_SIZE` values.

Since the constraints of the norm establishes that a function body must have 25 lines of code at maximum, we would have to split some of the `get_next_line` functionalities into two functions. The end result is this:

```
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
	int	n;

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
	static char buf[FD_MAX][BUFFER_SIZE + 1];
	
	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	return (read_line(fd, buf));
}
```

The auxiliary functions are not changed, and the inner logic presented on the sections before remain. The difference this time is that the main logic is now handled by a function called `read_line`, and in `get_next_line` we run a conditional check before calling it. 

This conditional check is imposed so to validate edge cases, in which the multiple FD's may represent access to invalid files or the conditions in which we deal with the multiple files are not valid.

But the real issue comes down due to the project's logic being depandable of a static `buf` definition. 

`buf` is now a two-dimension array, first being the FD, and the last one representing the lines.

In practice, this can create problems that are memory-related. If we were to create an array that can process thousands of files, when we set the `BUFFER_SIZE` to handle millions of bytes by chunk, the memory allocated can be as close to almost all available RAM Memory space available - rendering the program useless since the computer has a failsafe routine to prevent running such memory degrading programs.

We can represent the declaration of our `buf` as an equation:

```
FD * (BUFFER_SIZE + 1) = Memory Allocated
```

If we are to deal with a threshold to memory allocation, that imposes a particular relationship between our variables `FD` and `BUFFER_SIZE`. So, the problem can be solved rearranging the equation:

```
FD = (Memory Allocated) / (BUFFER_SIZE + 1)
```

What is important to get out from this is that `FD` has now an _inverse proportinality relationship_ with `BUFFER_SIZE` - if we were to grow `BUFFER_SIZE`, we'd have to shrink `FD`.

Since I don't know exactly the limit to which is possible to allocate memory safely, I've made the MACRO `FD_MAX` to follow a conditional logic: _if the program is dealing with at least 1MB per chunk, we will deal only with a value as close as possible to the FD limit within C language constraints._

This limit is discussed in the article I've wrote, so go check it out!


