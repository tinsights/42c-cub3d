/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <tjegades@student.42singapore.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:58:20 by sthiagar          #+#    #+#             */
/*   Updated: 2025/02/20 17:25:29 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*getstr(char *str, int rwidth)
{
	int		len;
	int		i;
	char	*arr;

	len = ft_strlen(str);
	arr = ft_calloc(rwidth + 1, sizeof(char));
	if (arr == NULL)
		return (NULL);
	i = -1;
	while (++i < len)
	{
		arr[i] = str[i];
		if (str[i] == ' ')
			arr[i] = '1';
		else if (str[i] != '0' && str[i] != '1' && str[i] != 'D')
			arr[i] = '0';
	}
	while (i < rwidth)
		arr[i++] = 'a';
	return (arr);
}

void	make_maparray(t_mapdata *mi, char **arr)
{
	t_list	*head;
	t_list	*next;
	int		i;

	head = mi->lst;
	i = 0;
	while (head != NULL)
	{
		next = (*head).next;
		arr[i] = getstr((*head).content, mi->rwidth);
		if (arr[i] == NULL)
		{
			free_strarr(arr);
			break ;
		}
		head = next;
		arr[++i] = NULL;
	}
}

static int	makelist(t_list	**head, char *line, int *rows)
{
	t_list	*node;

	if (isemptyline(line))
		return (0);
	if (remove_nl(&line) == -1)
		return (0);
	node = ft_lstnew(line);
	if (node == NULL)
		return (0);
	ft_lstadd_back(head, node);
	*rows += 1;
	return (1);
}

int	get_tmaplist(int fd, t_mapdata *mi)
{
	char	*line;
	t_list	*head;

	head = NULL;
	line = get_next_line(fd);
	while (line && isemptyline(line))
	{
		free_str(&line);
		line = get_next_line(fd);
	}
	while (line)
	{
		if (makelist(&head, line, &mi->rows) == 0)
			break ;
		line = get_next_line(fd);
	}
	if (line != NULL)
	{
		free_maplst(&head);
		free_str(&line);
		get_next_line(-1);
		return (-1);
	}
	mi->lst = head;
	return (1);
}
