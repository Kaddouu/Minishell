/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:45:27 by ysaadaou          #+#    #+#             */
/*   Updated: 2024/11/23 16:45:30 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line_bonus.h"

char	*ft_sendline(char *s)
{
	char	*sendline;
	int		i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	sendline = malloc(sizeof(char) * i + 2);
	if (!sendline)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '\n')
	{
		sendline[i] = s[i];
		i++;
	}
	if (s[i] == '\n')
		sendline[i++] = '\n';
	sendline[i] = '\0';
	return (sendline);
}

char	*ft_keepline(char *s)
{
	char	*tokeep;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	while (s[i] && s[i] != '\n')
		i++;
	if (!s[i++])
		return (free(s), NULL);
	tokeep = malloc(sizeof(char) * (ft_strlen(s) - i + 1));
	if (!tokeep)
		return (free(s), NULL);
	while (s[i])
		tokeep[j++] = s[i++];
	tokeep[j] = '\0';
	free(s);
	if (!tokeep[0])
		return (free(tokeep), NULL);
	return (tokeep);
}

int	ft_free(int bytes_read, char *getline)
{
	if (bytes_read == 0 && (!getline || !getline[0]))
	{
		if (!getline)
			free(getline);
		return (0);
	}
	return (1);
}

int	ft_freee(int bytes_read, char *getline, char *buffer)
{
	if (bytes_read < 0)
	{
		free(getline);
		free(buffer);
		return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*getline[FD_MAX];
	char		*buffer;
	int			bytes_read;

	if (BUFFER_SIZE <= 0 || fd < 0 || fd > FD_MAX)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr_gnl(getline[fd]) && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (!ft_freee(bytes_read, getline[fd], buffer))
			return (NULL);
		buffer[bytes_read] = '\0';
		if (bytes_read > 0)
			getline[fd] = ft_strjoin_gnl(getline[fd], buffer);
	}
	free(buffer);
	if (!ft_free(bytes_read, getline[fd]))
		return (NULL);
	buffer = ft_sendline(getline[fd]);
	getline[fd] = ft_keepline(getline[fd]);
	return (buffer);
}
