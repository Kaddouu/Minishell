/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:43:04 by ysaadaou          #+#    #+#             */
/*   Updated: 2025/02/08 18:00:21 by ysaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef FD_MAX
#  define BUFFER_SIZE 42
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <stddef.h>
# include <stdint.h>

int		ft_strlen_gnl(char *s);
char	*ft_strjoin_gnl(char *s1, char *s2);
int		ft_strchr_gnl(char *s);
char	*ft_sendline(char *s);
char	*ft_keepline(char *s);
char	*get_next_line(int fd);

#endif
