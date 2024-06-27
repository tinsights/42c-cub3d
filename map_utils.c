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

int	remove_nl(char **str)
{
	char	**elem;

	elem = ft_split(*str, '\n');
	if (elem == NULL)
		return (-1);
	free (*str);
	*str = ft_strdup(elem[0]);
	free_strarr(elem);
	return (0);
}

int	isemptyline(char *str)
{
	char	**elem;
	int		count;

	elem = ft_split2(str, " \n");
	if (elem == NULL)
		return (1);
	count = wcount(elem);
	free_strarr(elem);
	if (count == 0)
		return (1);
	return (0);
}

void	update_pos_heading(t_input *dat, int row, int col, char c)
{
	dat->xpos = col;
	dat->ypos = row;
	if (c == 'N')
		dat->heading = 0;
	else if (c == 'S')
		dat->heading = M_PI;
	else if (c == 'W')
		dat->heading = 3 * M_PI / 2;
	else
		dat->heading = M_PI / 2;
}

int	validate_spawn(const char *str, int row, t_input *dat)
{
	int		len;
	char	c;
	int		cflag;
	int		col;

	cflag = 0;
	len = ft_strlen(str);
	col = 0;
	while (col < len)
	{
		c = str[col];
		if (validchr(c, "NSWE") == 1)
		{
			cflag++;
			if (cflag > 1)
				return (-1);
			else
				update_pos_heading(dat, row, col, c);
		}
		col++;
	}
	return (cflag);
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
