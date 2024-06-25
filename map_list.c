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

//map of regular shape needs each row in uniform width
//returns a new str of rwidth len
//fills each index with str value, except if there is space
//space is filled with 1 or 0 (CONSTANT)
//if str len < rwidth, the balance indexes are filled with 'a'
static char	*getstr(char *str, int rwidth)
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
	i = -1;
	while(++i < len)
	{
		arr[i] = str[i];
		if (str[i] == ' ')
			arr[i] = '1';
		else if (str[i] != '0' && str[i] != '1' && str[i] != 'D')//NSWE
			arr[i] = '0';
	}
	while (i < rwidth)
		arr[i++] = 'a';
	arr[i] = '\0';
	return(arr);
}

//helper function to tmap_to_array
static void make_maparray(t_mapdata *mi, char **arr)
{
    t_list	*head;
	t_list	*next;
	int	i;

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
		arr[++i] = NULL;//this is i+1 index of previous
	}
}

//convert the temporary map list after validation to char **map
char	**tmap_to_array(t_mapdata *mi)
{
	char	**arr;
	
	arr = (char **)malloc(sizeof(char *) * (mi->rows + 1));
	if (arr == NULL)
	{
		free_maplst(&mi->lst);
		return (NULL);
	}
    make_maparray(mi, arr);
	free_maplst(&mi->lst);
	return (arr);
}

//performs only 1 validation. (other validations require full scope of map)
//a) map string can't be an just spaces
//b) terminating \n in str not required. remove it.
//c) add to linked list
static int  makelist(t_list	**head, char *line)
{
    t_list	*node;
    
    if (isemptyline(line)) //a
		return (0);
    if (remove_nl(&line) == -1) //b
        return (0);
    node = ft_lstnew(line);
    if (node == NULL)
        return (0);
    ft_lstadd_back(head, node); //c
    return (1);
}

//build a temporary list of map strings
//calls makelist to add a node to linked list
int	get_tmaplist(int fd, t_mapdata *mi)
{
	char	*line;
	t_list	*head;
	
	head = NULL;
	line = get_next_line(fd, 0);
	while(line)
	{
        if (makelist(&head, line) == 0)
            break; //line != NULL
		mi->rows++;
		line = get_next_line(fd, 0);
	}
	if (line != NULL) //eof not reached, but error in current line
	{
		free_maplst(&head);
		free_str(&line);
		return (-1);
	}
	mi->lst = head;
	return (1);
}