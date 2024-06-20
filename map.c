/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 20:10:47 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/13 20:11:11 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	update_playerpos(t_mapinfo *mi, t_input *dat)
{
	t_list	*lst;
	int	flag;
	int	valid;
	
	flag = 0;
	mi->irow = 0;
	lst = mi->lst;
	while (lst)
	{
		mi->irow++;
		valid = validate_nswe((char *)lst->content, mi, dat);
		if (valid == -1 || (valid == 1 && flag == 1))
			return (-1);
		else if (valid == 1 && flag == 0)
			flag = 1;
		lst = lst->next;
	}
	if (flag == 0)
		return (-1);
	return (1);
}

int	isvalidchars(t_mapinfo *mi)
{
	t_list	*lst;
	int	max;

	max = 0;
	lst = mi->lst;
	while (lst)
	{
		max = ft_strlen((char *)lst->content);
		if (max > mi->rwidth)
			mi->rwidth = max;
		if (!validstr((char *)lst->content, "\n 10NSWE"))
			return (-1);
		lst = lst->next;
	}
	return (0);
}

int	maplist(int fd, t_mapinfo *mi)
{
	char	*line;
	t_list	*head;
	t_list	*node;
	
	head = NULL;
	line = get_next_line(fd);
	while(line)
	{
		if (isemptyline(line))
			return(free_return1(&head, &line));
		if (remove_nl(&line) == -1)
			return(free_return1(&head, &line));
		node = ft_lstnew(line);
		if (node == NULL)
			return(free_return1(&head, &line));
		ft_lstadd_back(&head, node);
		mi->rows++;
		line = get_next_line(fd);
	}
	mi->lst = head;
	return (1);
}

/*
char	**tmap_to_array(t_mapinfo *mi)//modified below
{
	char	**arr;
	t_list	*lst;
	int	i;
	
	arr = (char **)malloc(sizeof(char *) * (mi->rows + 1));
	if (arr == NULL)
		return (NULL);
	lst = mi->lst;
	//printf("rwidth is %d\n", mi->rwidth);
	i = 0;
	while (mi->lst != NULL)//malloc for each row
	{
		//arr[i] = (char *)malloc(sizeof(char) * (mi->rwidth + 1));
		lst = (*mi->lst).next;
		arr[i] = (char *)(*mi->lst).content;
		//arr[i] = temp((*mi->lst).content, rwidth);
		//free((*mi->lst).content);
		free(mi->lst);
		mi->lst = lst;
		i++;
	}
	free(mi->lst);
	mi->lst = NULL;
	arr[i] = NULL;
	return (arr);
} */

char	*getstr(char *str, int rwidth)
{
	int	len;
	int	bal;
	int	i;
	char	*arr;
	
	len = ft_strlen(str);
	bal = rwidth - len;
	arr = (char *)malloc(sizeof(char) * rwidth + 1);
	if (arr == NULL)
		return (NULL);
	i = 0;
	while(i < len)
	{
		arr[i] = str[i];
		if (str[i] == ' ')
			arr[i] = ONEORZERO;
		else if (str[i] != '0' && str[i] != '1')//NSWE
			arr[i] = '0';
		i++;
		
	}
	while (i < rwidth)
	{
		arr[i] = 'a';
		i++;
	}
	arr[i] = '\0';
	return(arr);
}

char	**tmap_to_array(t_mapinfo *mi)
{
	char	**arr;
	t_list	*lst;
	int	i;
	
	arr = (char **)malloc(sizeof(char *) * (mi->rows + 1));
	if (arr == NULL)
		return (NULL);
	lst = mi->lst;
	i = 0;
	while (mi->lst != NULL)//malloc for each row
	{
		lst = (*mi->lst).next;
		arr[i] = getstr((*mi->lst).content, mi->rwidth);
		if (arr[i] == NULL)
			free_strarr2(arr, i);
		free((*mi->lst).content);
		free(mi->lst);
		mi->lst = lst;
		i++;
	}
	free(mi->lst);
	mi->lst = NULL;
	arr[i] = NULL;
	return (arr);
}

int	init_mapinfo(t_mapinfo *mi, int fd)
{
	mi->rows = 0;
	mi->rwidth = 0;
	mi->nswe[0][0] = 78;//n
	mi->nswe[0][1] = 0;
	mi->nswe[0][2] = -1;
	mi->nswe[0][3] = 90;

	mi->nswe[1][0] = 83;//s
	mi->nswe[1][1] = 0;
	mi->nswe[1][2] = 1;
	mi->nswe[1][3] = 270;

	mi->nswe[2][0] = 87;//w
	mi->nswe[2][1] = -1;
	mi->nswe[2][2] = 0;
	mi->nswe[2][3] = 180;

	mi->nswe[3][0] = 69;//e
	mi->nswe[3][1] = 1;
	mi->nswe[3][2] = 0;
	mi->nswe[3][3] = 0;

	if (maplist(fd, mi) == -1)
		return (-1);
	return (0);
}

int	parse_map(int fd, t_input *dat)
{
	t_mapinfo	mi;
	
	if (init_mapinfo(&mi, fd) == -1)
		return (-1);
	if (isvalidchars(&mi) == -1)
		return (free_maplst(&mi.lst));
	if (update_playerpos(&mi, dat) == -1)
		return (free_maplst(&mi.lst));
	if (isvalidborder(&mi, dat) == -1)
		return (free_maplst(&mi.lst));
	dat->map = tmap_to_array(&mi);//tmap refers to lst
	if (dat->map == NULL)
		return (-1);
	return (0);
}
