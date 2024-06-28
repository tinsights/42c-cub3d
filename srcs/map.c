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

void	make_maparray(t_mapdata *mi, char **arr);

static char	**tmap_to_array(t_mapdata *mi)
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

static int	update_playerspawn(t_mapdata *mi, t_input *dat)
{
	t_list	*lst;
	int		counter;
	int		valid;
	int		row;

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

static int	allvalidchars(t_mapdata *mi)
{
	t_list	*lst;
	int		max;

	max = 0;
	lst = mi->lst;
	while (lst)
	{
		max = ft_strlen((char *)lst->content);
		if (max > mi->rwidth)
			mi->rwidth = max;
		if (!validstr((char *)lst->content, "\n 10NSWED"))
			return (-1);
		lst = lst->next;
	}
	return (0);
}

static int	init_mapdata(t_mapdata *mi, int fd)
{
	mi->rows = 0;
	mi->rwidth = 0;
	if (get_tmaplist(fd, mi) == -1)
	{
		get_next_line(-1);
		return (-1);
	}
	return (0);
}

int	parse_map(int fd, t_input *dat)
{
	t_mapdata	mi;
	int			isvalid;

	isvalid = 1;
	if (init_mapdata(&mi, fd) == -1)
		return (-1);
	if (isvalid && allvalidchars(&mi) == -1)
		isvalid = 0;
	if (isvalid && update_playerspawn(&mi, dat) == -1)
		isvalid = 0;
	if (isvalid && isvalidborder(&mi, dat) == -1)
		isvalid = 0;
	if (!isvalid)
	{
		free_maplst(&mi.lst);
		return (-1);
	}
	dat->map = tmap_to_array(&mi);
	if (dat->map == NULL)
		return (-1);
	dat->mwidth = mi.rwidth;
	dat->mheight = mi.rows;
	return (0);
}
