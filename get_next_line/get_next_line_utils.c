/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 13:14:42 by sthiagar          #+#    #+#             */
/*   Updated: 2023/11/10 19:26:54 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;
	size_t	i;

	i = 0;
	arr = (void *)malloc(nmemb * size);
	if (arr == NULL)
		return (NULL);
	else
	{
		while (i < (nmemb * size))
		{
			((unsigned char *)arr)[i] = 0;
			i++;
		}
		return (arr);
	}
}

int	store_buffer(t_buffer_lst *lst, t_buffer_str *buf)
{
	t_buffer_node	*new;
	t_buffer_node	*cur;

	cur = NULL;
	new = (t_buffer_node *)ft_calloc(1, sizeof(t_buffer_node));
	if (new == NULL)
		return (-1);
	lst->len += buf->len;
	new->buffer.len = buf->len;
	new->buffer.pos = buf->pos;
	new->buffer.str = buf->str;
	new->next = NULL;
	if (lst->node != NULL)
	{
		cur = lst->node;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
	else
		lst->node = new;
	return (1);
}

char	*free_lst(t_buffer_lst *lst, int fd, int status)
{
	t_buffer_node	*temp;

	lst->len = 0;
	while (lst->node)
	{
		temp = lst->node->next;
		if (lst->node->buffer.str != NULL)
			free(lst->node->buffer.str);
		free(lst->node);
		lst->node = temp;
	}
	if (status == -1)
	{
		close(fd);
		write(1, "Error\n", 6);
		perror("Read");
	}
	return (NULL);
}

void	strcpy_all(char *output, t_buffer_lst *lst, t_buffer_str *buf, int flg)
{
	int				i;
	int				j;
	t_buffer_node	*cur;

	cur = lst->node;
	i = 0;
	while (cur && flg != 9)
	{
		j = 0;
		while (cur->buffer.str[j] != '\0')
		{
			output[i] = cur->buffer.str[j];
			i++;
			j++;
		}
		cur = cur->next;
	}
	j = 0;
	while (buf->len > 0 && j < buf->pos)
	{
		output[i] = buf->str[j];
		i++;
		j++;
	}
}
