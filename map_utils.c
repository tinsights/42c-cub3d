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

//separate newline from str using ft_split
//return the first part of the split, which has no \n
int	remove_nl(char **str)
{
	char	**elem;
	
	elem = ft_split(*str, '\n');//delimiter \n
	if (elem == NULL)
		return (-1);
	free (*str);
	*str = ft_strdup(elem[0]); //you want to free char **elem
	free_strarr(elem);
	return (0);
}

//a)line has no space, but only newline.
//b)line has space, followed by newline.
//a) and b) is valid between texture
//a) is invalid if between map str
//b) is valid (filled with 0 or 1) between map str

//if str contains spaces and /n or /n only it is empty line
int	isemptyline(char *str)
{
	char	**elem;
	int	count;
	
	elem = ft_split2(str, " \n");//delimiter space, \n
	if (elem == NULL)
		return (1); //wrong!! it should return -1
	count = wcount(elem);
	free_strarr(elem);
	if (count == 0)				//if word count = 0 means empty line
		return (1);
	return (0);		
}

//row and col where N S W or E was found
//update rad depending on N S W or E
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
	else // E
		dat->heading = M_PI / 2;
}

//row is the current row number of this uninspected str
//inspects str in current row for presence of N S W E
//invalid if there is more than one N S W or E
//if there is only one N S W or E, update the position of row and col
// col is the matched char index in the str
int	validate_spawn(const char *str, int row, t_input *dat)
{
	int	len;
	char c;
	int	cflag;
	int	col;

	cflag = 0;
	len = ft_strlen(str);
	col = 0;
	while (col < len) 						//for each char in str
	{ 	
		c = str[col];
		if (validchr(c, "NSWE") == 1)		//current char is N S W or E
		{
			cflag++;						//invalid if cflag > 1
			if (cflag > 1)					// dup found in the same line
				return (-1);
			else
				update_pos_heading(dat, row, col, c);	
		}
		col++;
	}
	return (cflag);//0 none, 1 found
}

//number of element is the list
//lst is the temporary map list
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