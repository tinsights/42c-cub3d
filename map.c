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

//a) number of rows is counted here
//b) validate there is only one (N S W E) in any one row
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
		valid = validate_spawn((char *)lst->content, row, dat); //b)
		if (valid == -1 || (valid == 1 && counter == 1))
			return (-1);
		else if (valid == 1 && counter == 0)
			counter = 1;
		lst = lst->next;
		row++; //a)
	}
	if (counter == 0)
		return (-1);
	return (1);
}

//a) each map line str can be 1 0 (N S W E) or D 
// each string can have varying length
// b) maximum row width is updated here.
int	allvalidchars(t_mapdata *mi)
{
	t_list	*lst;
	int	max;

	max = 0;
	lst = mi->lst;
	while (lst)
	{
		max = ft_strlen((char *)lst->content);
		if (max > mi->rwidth) //b)
			mi->rwidth = max;
		if (!validstr((char *)lst->content, "\n 10NSWED"))//a)
			return (-1);
		lst = lst->next;
	}
	return (0);
}

int	init_mapdata(t_mapdata *mi, int fd)
{
	mi->rows = 0;
	mi->rwidth = 0;
	if (get_tmaplist(fd, mi) == -1)
		return (-1);
	return (0);	
}

//a) valid letter in each line of map strings
//b) there can only be 1 N S W E in the map
//c) border enclosed with 1, Player within border
//d) finally, map rows, cols and map itself is updated
int	parse_map(int fd, t_input *dat)
{
	t_mapdata	mi;
	int	isvalid;
	
	isvalid = 1;
	if (init_mapdata(&mi, fd) == -1)
		return (-1);
	if (allvalidchars(&mi) == -1) //a)
		isvalid = 0;
	if (update_playerspawn(&mi, dat) == -1)//b)
		isvalid = 0;
	if (isvalidborder(&mi, dat) == -1)//c) 
		isvalid = 0;
	if (!isvalid)
	{
		free_maplst(&mi.lst);
		return (-1);
	}
	dat->map = tmap_to_array(&mi);//tmap refers to lst
	if (dat->map == NULL)
		return (-1);
	dat->mwidth = mi.rwidth; //d)
	dat->mheight = mi.rows;
	return (0);
}