/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapborder.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 19:26:35 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/17 19:26:37 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//filling one row at a time on the temp map array
//fill row[0] and row[last] with -1
//fill with actual values for the length of the str
//if len < col (row width), fill balance with -1
//inside the valid len of the str, 4 things happen
// 1) if there is space fill it with 0 or 1
// 2) if there is D, fill it with 2
// 3) if there is NSWE, fill it with 0
// 4) otherwise fill with actual value.
void	fill_tmap(int row[], int col, char *str)
{
	int	i;
	int	len;
	
	len = 0;
	if (str)
		len = ft_strlen(str);
	row[0] = -1;		//column 1 is -1
	i = 0;
	while (i < len)
	{
		row[i + 1] = str[i] - 48;	//0 or 1
		if (str[i] == ' ')		//space
			row[i + 1] = 1;
		else if (str[i] == 'D')
			row[i + 1] = 2; //DOOR
		else if (str[i] != '0' && str[i] != '1')//NSWE
			row[i + 1] = 0;
		i++;
	}
	while (i < col - 1)
	{
		row[i + 1] = -1;
		i++;
	}
	row[col - 1] = -1;	//last column is -1
}

//cells outside of proper map is filled with -1
//start from player position, move through 0s in all direction
//if I land on 1, don't step in.
//if I step into a cell with -1, the wall is breached
int	isenclosedwall(int **tmap, t_mapdata *mi, int row, int col)
{
	if (tmap[row][col] == -1)
		return (-1);
	if (tmap[row][col] == 1 || tmap[row][col] == 2)
		return (0);
	if (tmap[row][col] == 0)
		tmap[row][col] = 1;
	if (isenclosedwall(tmap, mi, row - 1, col) == -1)
		return (-1);
	if (isenclosedwall(tmap, mi, row + 1, col) == -1)
		return (-1);
	if (isenclosedwall(tmap, mi, row, col - 1) == -1)
		return (-1);
	if (isenclosedwall(tmap, mi, row, col + 1) == -1)
		return (-1);
	return (0);
}

//allocate memory for int array of enlarged size of row and cols
//if allocation fails at row, free what was already allocated, return NULL
int	**alloc_tmap(int rows, int cols)
{
	int	**tmap;
	int	i;
	
	tmap = (int **)malloc((rows) * sizeof(int *));
	if (tmap == NULL)
		return (NULL);
	i = 0;
	while (i < rows)
	{
		tmap[i] = (int *)malloc((cols) * sizeof(int));
		if (tmap[i] == NULL)
		{
			free_intarr(tmap, i);
			return (NULL);
		}
		i++;
	}
	return (tmap);
}

//a) create a temp map array with additional top and bottom row
//   additional left and right column
//b) check is wall is enclosed
//c) free temp map array
int	isvalidborder(t_mapdata *mi, t_input *dat)
{
	t_list	*lst;
	int	**tmap;
	int	i;
	int	ret;

	tmap = alloc_tmap(mi->rows + 2, mi->rwidth + 2); //a
	if (tmap == NULL)
		return (-1);
	lst = mi->lst;
	i = 0;
	while (i < mi->rows + 2)
	{
		//1st and last row fill with 'a'
		if (i == 0 || i == mi->rows + 1)
			fill_tmap(tmap[i], mi->rwidth + 2, NULL);
		//rows inbetween fill 1st and last col with 'a', rest actual val
		else
		{
			fill_tmap(tmap[i], mi->rwidth + 2, (char *)lst->content);
			lst = lst->next;
		}
		i++;
	}
	ret = isenclosedwall(tmap, mi, dat->ypos, dat->xpos); //b
	free_intarr(tmap, mi->rows + 2);//c
	return (ret);
}

