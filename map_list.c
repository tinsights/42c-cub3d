/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:58:20 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/25 20:58:31 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*getstr(char *str, int rwidth)
{
	int		len;
	int		bal;
	int		i;
	char	*arr;

	len = ft_strlen(str);
	bal = rwidth - len;
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

static void	make_maparray(t_mapdata *mi, char **arr)
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

char	**tmap_to_array(t_mapdata *mi)
{
	char	**arr;

	arr = ft_calloc((mi->rows + 1), sizeof(char *));
	if (arr == NULL)
	{
		free_maplst(&mi->lst);
		return (NULL);
	}
	make_maparray(mi, arr);
	free_maplst(&mi->lst);
	return (arr);
}

static int	makelist(t_list	**head, char *line)
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
	return (1);
}

int	get_tmaplist(int fd, t_mapdata *mi)
{
	char	*line;
	t_list	*head;

	head = NULL;
	line = get_next_line(fd, 0);
	while (line)
	{
		if (makelist(&head, line) == 0)
			break ;
		mi->rows++;
		line = get_next_line(fd, 0);
	}
	if (line != NULL)
	{
		free_maplst(&head);
		free_str(&line);
		return (-1);
	}
	mi->lst = head;
	return (1);
}
