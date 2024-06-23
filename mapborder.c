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

void	fill_tmap(int row[], int col, char *str)
{
	int	i;
	int	len;
	
	len = 0;
	if (str)
		len = ft_strlen(str);
	row[0] = -1;		//column 1 is -1
	row[col - 1] = -1;	//last column is -1
	i = 0;
	while (i < len)
	{
		row[i + 1] = str[i] - 48;	//0 or 1
		if (str[i] == ' ')		//space
			row[i + 1] = ONEORZERO;
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
}

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

int	isvalidborder(t_mapdata *mi, t_input *dat)
{
	t_list	*lst;
	int	**tmap;
	int	i;
	int	ret;

	tmap = alloc_tmap(mi->rows + 2, mi->rwidth + 2);
	if (tmap == NULL)
		return (-1);
	lst = mi->lst;
	i = 0;
	while (i < mi->rows + 2)
	{
		if (i == 0 || i == mi->rows + 1) //1st and last row
			fill_tmap(tmap[i], mi->rwidth + 2, NULL);
		else//rows inbetween
		{
			fill_tmap(tmap[i], mi->rwidth + 2, (char *)lst->content);
			lst = lst->next;
		}
		i++;
	}
	ret = isenclosedwall(tmap, mi, dat->ypos, dat->xpos);
	free_intarr(tmap, mi->rows + 2);
	return (ret);
}

