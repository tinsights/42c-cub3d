/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 09:46:37 by sthiagar          #+#    #+#             */
/*   Updated: 2023/11/17 17:31:39 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	update_lst(t_buffer_lst *lst, t_buffer_str *buf, int status)
{
	char	*new_str;
	int		index;

	new_str = (char *)ft_calloc(buf->len - buf->pos + 1, sizeof(char));
	if (new_str == NULL)
	{
		free (buf->str);
		return (-1);
	}
	index = 0;
	while (index < (buf->len - buf->pos))
	{
		new_str[index] = buf->str[index + buf->pos];
		index++;
	}
	free (buf->str);
	buf->str = new_str;
	buf->len = buf->len - buf->pos;
	buf->pos = 0;
	if (status == 9)
		return (1);
	free_lst(lst, 0, 0);
	return (store_buffer(lst, buf));
}

static char	*get_frm_lst_buf(t_buffer_lst *lst, t_buffer_str *buf, int status)
{
	char			*output;

	if (lst->node == NULL && buf->str == NULL)
		return (NULL);
	output = (char *)ft_calloc(lst->len + buf->pos + 1, sizeof(char));
	if (output == NULL)
		return (free_lst(lst, 0, 0));
	strcpy_all(output, lst, buf, status);
	if (status == 0)
		free_lst(lst, 0, 0);
	else if (buf->len > 0 && buf->len == buf->pos)
	{
		free(buf->str);
		buf->str = NULL;
		free_lst(lst, 0, 0);
	}
	else if (buf->len > 0 && buf->len > buf->pos)
	{
		if (update_lst(lst, buf, status) == -1)
			return (NULL);
	}
	return (output);
}

static char	*get_frm_lst_only(t_buffer_lst *lst, t_buffer_str *buf)
{
	int	i;

	buf = &lst->node->buffer;
	i = 0;
	while (buf->str[i] != '\0')
	{
		if (buf->str[i] == '\n')
		{
			lst->len = 0;
			buf->pos = i + 1;
			return (get_frm_lst_buf(lst, buf, 9));
		}
		i++;
	}
	lst->len = buf->len;
	return (NULL);
}

static int	read_buffer(t_buffer_lst *lst, int fd, t_buffer_str *buf)
{
	buf->len = 0;
	buf->str = (char *)ft_calloc(BUFFER_SIZE + 1, (sizeof(char)));
	if (buf->str == NULL)
		return (-1);
	buf->len = read(fd, buf->str, BUFFER_SIZE);
	if (buf->len == -1 || buf->len == 0)
	{
		free(buf->str);
		buf->str = NULL;
		if (buf->len == -1)
			return (-1);
		return (0);
	}
	buf->pos = 0;
	while (buf->str[buf->pos] != '\0')
	{
		if (buf->str[buf->pos++] == '\n')
			return (2);
	}
	buf->pos = 0;
	return (store_buffer(lst, buf));
}

char	*get_next_line(int fd, int lastcall)
{
	static t_buffer_lst	lst = {0, NULL};
	t_buffer_str		buf;
	char				*output;
	int					read_status;

	output = NULL;
	buf.len = 0;
	buf.pos = 0;
	if (fd < 0 || BUFFER_SIZE <= 0 || lastcall == 1)
		return (free_lst(&lst, fd, 0));
	if (lst.node != NULL)
	{
		output = get_frm_lst_only(&lst, &buf);
		if (output != NULL)
			return (output);
	}
	read_status = 1;
	while (read_status == 1)
		read_status = read_buffer(&lst, fd, &buf);
	if (read_status == -1)
		return (free_lst(&lst, fd, -1));
	else if (read_status == 0 || read_status == 2)
		output = get_frm_lst_buf(&lst, &buf, read_status);
	return (output);
}
