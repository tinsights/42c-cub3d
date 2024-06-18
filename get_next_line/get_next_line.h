/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 09:33:36 by sthiagar          #+#    #+#             */
/*   Updated: 2023/11/13 23:17:00 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_buffer_str
{
	int		len;
	int		pos;
	char	*str;
}	t_buffer_str;

typedef struct s_buffer_node
{
	t_buffer_str			buffer;
	struct s_buffer_node	*next;
}	t_buffer_node;

typedef struct s_buffer_lst
{
	int				len;
	t_buffer_node	*node;
}	t_buffer_lst;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

char	*get_next_line(int fd);
void	*ft_calloc(size_t nmemb, size_t size);
int		store_buffer(t_buffer_lst *lst, t_buffer_str *buf);
char	*free_lst(t_buffer_lst *lst, int fd, int status);
void	strcpy_all(char *output, t_buffer_lst *lst, t_buffer_str *buf, int flg);
#endif
