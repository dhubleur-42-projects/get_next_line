/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:12:01 by dhubleur          #+#    #+#             */
/*   Updated: 2021/12/02 17:13:06 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
Read BUFFER_SIZE chars from the fd.
Append a \0 at the end of this readed str
Append the last stocked str (persistant storage) with this readed str
Continue make this while an \n is found or the EOF is reached (read 0 chars)
*/
char	*read_and_stock(int fd, char *storage_buffer)
{
	char	*reading_buffer;
	int		readed;

	reading_buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!reading_buffer)
		return (NULL);
	readed = 1;
	while (!ft_strchr(storage_buffer, '\n') && readed != 0)
	{
		readed = read(fd, reading_buffer, BUFFER_SIZE);
		reading_buffer[readed] = '\0';
		storage_buffer = ft_strjoin(storage_buffer, reading_buffer);
	}
	free(reading_buffer);
	return (storage_buffer);
}

/*
Search a \n or the end of the file anc calcul the length of this line
Malloc a str of this size (line)
Copy all the chars and the \n if needed
Null terminate this string
*/
char	*get_line(char *storage_buffer)
{
	int		i;
	char	*line;

	i = 0;
	if (!storage_buffer[i])
		return (NULL);
	while (storage_buffer[i] && storage_buffer[i] != '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (storage_buffer[i] && storage_buffer[i] != '\n')
	{
		line[i] = storage_buffer[i];
		i++;
	}
	if (storage_buffer[i] == '\n')
	{
		line[i] = storage_buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

/*
Search a \n or the end of the file
If there is no more chars in the buffer, free him
Malloc a new buffer with the size of the ancien - the size of the line
Copy to the new buffer the chars they are not in the line
Free ancin buffer and return the new for persistant storage
*/
char	*clear_storage(char *storage_buffer)
{
	int		i;
	int		j;
	char	*buff2;

	i = 0;
	while (storage_buffer[i] && storage_buffer[i] != '\n')
		i++;
	if (!storage_buffer[i])
	{
		free(storage_buffer);
		return (NULL);
	}
	buff2 = (char *)malloc(sizeof(char) * (ft_strlen(storage_buffer) - i + 1));
	if (!buff2)
		return (NULL);
	i++;
	j = 0;
	while (storage_buffer[i])
		buff2[j++] = storage_buffer[i++];
	buff2[j] = '\0';
	free(storage_buffer);
	return (buff2);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*storage_buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) == -1)
		return (NULL);
	storage_buffer = read_and_stock(fd, storage_buffer);
	if (!storage_buffer)
		return (NULL);
	line = get_line(storage_buffer);
	storage_buffer = clear_storage(storage_buffer);
	return (line);
}
