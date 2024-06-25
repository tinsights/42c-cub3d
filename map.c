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

int	update_playerspawn(t_mapdata *mi, t_input *dat)
{
	t_list	*lst;
	int	counter;
	int	valid;
	int	row;
	
	counter = 0;
	row = 0;
	lst = mi->lst;
	while (lst)
	{
		valid = validate_spawn((char *)lst->content, row, dat);
		if (valid == -1 || (valid == 1 && counter == 1))
			return (-1);
		else if (valid == 1 && counter == 0)
			counter = 1;
		lst = lst->next;
		row++;
	}
	if (counter == 0)
		return (-1);
	return (1);
}

int	allvalidchars(t_mapdata *mi)
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
		if (!validstr((char *)lst->content, "\n 10NSWED"))//D door
			return (-1);
		lst = lst->next;
	}
	return (0);
}

int	maplist(int fd, t_mapdata *mi)
{
	char	*line;
	t_list	*head;
	t_list	*node;
	
	head = NULL;
	line = get_next_line(fd, 0);
	while(line)
	{

		if (isemptyline(line))
			break;
		if (remove_nl(&line) == -1)
			break;
		node = ft_lstnew(line);
		if (node == NULL)
			break;
		ft_lstadd_back(&head, node);
		mi->rows++;
		line = get_next_line(fd, 0);
	}
	if (line != NULL) //meaning eof not reached, some error encountered in between
	{
		free_maplst(&head);
		free_str(&line);
		return (-1);
	}
	mi->lst = head;
	return (1);
}

//returns a new str of rwidth len.
//fills each index with str value, except if there is space
//space is filled with 1 or 0 (CONSTANT)
//if str len < rwidth, the balance indexes are filled with 'a'
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
			arr[i] = '1';
		else if (str[i] != '0' && str[i] != '1' && str[i] != 'D')//NSWE
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

char	**tmap_to_array(t_mapdata *mi)
{
	char	**arr;
	t_list	*head;
	t_list	*next;
	int	i;
	
	arr = (char **)malloc(sizeof(char *) * (mi->rows + 1));
	if (arr == NULL)
	{
		free_maplst(&mi->lst);
		return (NULL);
	}
	head = mi->lst;
	i = 0;
	while (head != NULL)//malloc for each row
	{
		next = (*head).next;
		arr[i] = getstr((*head).content, mi->rwidth);
		if (arr[i] == NULL)
		{
			free_strarr2(arr, i);
			break;
		}
		head = next;
		i++;
		arr[i] = NULL;//this is i+1 index of previous
	}
	free_maplst(&mi->lst);
	return (arr);
}

int	init_mapdata(t_mapdata *mi, int fd)
{
	mi->rows = 0;
	mi->rwidth = 0;
	if (maplist(fd, mi) == -1)
		return (-1);
	return (0);	
}

int	parse_map(int fd, t_input *dat)
{
	t_mapdata	mi;
	
	if (init_mapdata(&mi, fd) == -1)
		return (-1);
	if (allvalidchars(&mi) == -1) //valid letters in map
	{
		free_maplst(&mi.lst);
		return (-1);
	}
	if (update_playerspawn(&mi, dat) == -1)//valid nswe
	{
		free_maplst(&mi.lst);
		return (-1);
	}
	if (isvalidborder(&mi, dat) == -1)
	{
		free_maplst(&mi.lst);
		return (-1);
	}
	dat->mwidth = mi.rwidth;
	dat->mheight = mi.rows;
	dat->map = tmap_to_array(&mi);//tmap refers to lst
	if (dat->map == NULL)
		return (-1);
	return (0);
}