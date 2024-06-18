/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:48:06 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/15 16:48:09 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	remove_nl(char **line)
{
	char	**elem;
	
	elem = ft_split2(*line, "\n");//delimiter \n
	if (elem == NULL)
		return (-1);
	free (*line);
	*line = ft_strdup(elem[0]);
	free_strarr(elem);
	return (0);
}

int	isemptyline(char *line)
{
	char	**elem;
	int	count;
	
	elem = ft_split2(line, " \n");//delimiter space, \n
	if (elem == NULL)
		return (1);
	count = wcount(elem);
	free_strarr(elem);
	if (count == 0)
		return (1);
	return (0);		
}

void	update_orient_pos(t_mapinfo *mi, t_input *dat, int j)
{
	dat->xpos = mi->icol + 1;
	dat->ypos = mi->irow;
	dat->xdir = mi->nswe[j][1]; 
	dat->ydir = mi->nswe[j][2];
}

int	validate_nswe(const char *str, t_mapinfo *mi, t_input *dat)
{
	int	i;
	int	len;
	int	cflag;

	cflag = 0;
	len = ft_strlen(str);
	mi->icol = 0;
	while (mi->icol < len)
	{ 	
		i = 0;
		while (i < 4)//loop on nswe
		{
			if (str[mi->icol] == mi->nswe[i][0])
			{
				cflag++;
				if (cflag > 1)// dup found
					return (-1);
				else
					update_orient_pos(mi, dat, i);		
			}
			i++;	
		}
		mi->icol++;
	}
	return (cflag);//0 none, 1 found
}

int	lst_count(t_list *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}


