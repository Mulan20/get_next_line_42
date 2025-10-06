/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:40:55 by nkham             #+#    #+#             */
/*   Updated: 2025/10/06 17:50:57 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

//This function will be used to read into buffer-passed
//This is creating a character array of buffer_size+ 1 
//to store the null terminator
//Then we will have the read_amt to check whether we have read 
//from fd successfully or not
//read_amt is set to 1 originatlly to at least read once or enter the loop
//if we didn't read anything into the buffer, the buffer would be free
//else we will put the null terminator at the end 
//Then we will join the buffer we read to the buffer_passed 
//Then we will be finding the new line character in the newly 
//allocated buffer_passed 
//free the local buffer and return the buffer_passed 
static char	*read_into_buffer(int fd, char *buffer_passed)
{
	char	*buffer;
	int		read_amt;

	buffer = malloc (BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	read_amt = 1;
	while (read_amt > 0)
	{
		read_amt = read(fd, buffer, BUFFER_SIZE);
		if (read_amt == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[read_amt] = '\0';
		buffer_passed = ft_strjoin(buffer_passed, buffer);
		if (ft_strchr(buffer_passed, '\n'))
			break ;
	}
	free(buffer);
	return (buffer_passed);
}

//this function is to extract the line from the buffer being passed
//if the buffer_passed is empty, we will return NULL
//we will be finding the length of the buffer being passed until it is newline
//and it is not empty
//if we reaches new line, we will copy that new line to line 
//and return that line
static char	*extract_line(char *buffer_passed)
{
	char	*line;
	int		counter;

	counter = 0;
	if (!buffer_passed[counter])
		return (NULL);
	while (buffer_passed[counter] && buffer_passed[counter] != '\n')
		counter++;
	if (buffer_passed[counter] == '\n')
		counter++;
	line = ft_substr(buffer_passed, 0, counter);
	return (line);
}

//this is used to clean the buffer_passed each time
//we will try to count until the buffer reaches the end of the line with null
//then, we will try to allocate the buffer with the leftoversize until this len
//Then we will copy the rest of the buffer after this
//then erase the original buffer
//return the copied buffer 
static char	*erase_buffer(char *buffer_passed)
{
	char	*buffer;
	int		oricnt;
	int		asscnt;

	oricnt = 0;
	while (buffer_passed[oricnt] && buffer_passed[oricnt] != '\n')
		oricnt++;
	if (!buffer_passed[oricnt])
	{
		free(buffer_passed);
		return (NULL);
	}
	oricnt++;
	buffer = malloc(ft_strlen(buffer_passed) - oricnt + 1);
	if (!buffer)
		return (NULL);
	asscnt = 0;
	while (buffer_passed[oricnt])
		buffer[asscnt++] = buffer_passed[oricnt++];
	buffer[asscnt] = '\0';
	free(buffer_passed);
	return (buffer);
}

//main function of get_next_line
//the file descriptor and the assigned buffer sizw would be checked 
//first and then
//using a read_into_buffer, we wil read the data into the buffer
//if the buffer is empty or nth is read, it should return NULL
//we will get the line from the buffer we read and then the buffer must 
//be erased back
//then return the line of the buffer
char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer[fd] = read_into_buffer(fd, buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	line = extract_line(buffer[fd]);
	buffer[fd] = erase_buffer(buffer[fd]);
	return (line);
}
