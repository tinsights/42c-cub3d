/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 19:55:14 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/18 19:55:18 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	print_lst(t_list *lst)
{
	printf("map list:\n");
	while (lst)
	{
		printf("%s\n",(char *)lst->content);
		lst = lst->next;
	}
	printf("end of list\n");
}

void	print_mi(t_mapinfo *mi)
{
	printf("\nprint map info: \n");
	printf("mi->rows %d\n",mi->rows);
	// printf("mi->cols %d\n",mi->cols);
	printf("mi->rwidth %d\n",mi->rwidth);
	print_lst(mi->lst);
}

void	print_dblarr(char **arr)
{
	int	i;
	
	i = 0;
	while(arr && arr[i])
	{
		printf("%s\n",arr[i]);
		i++;
	}
}

void	print_intarr(int **arr, int rows, int cols)
{
	int 	i;
	int	j;
	
	i = 0;
	while(i < rows)
	{
		j = 0;
		while (j < cols)
		{
			printf("%d",arr[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void	print_input(t_input *dat) // test function 
{
	printf("print input: \n");
	printf("NO xpm: %s\n",dat->nxpm);
	printf("SO xpm: %s\n",dat->sxpm);
	printf("EA xpm: %s\n",dat->expm);
	printf("WE xpm: %s\n",dat->wxpm);
	printf("floor color r: %d\n",dat->fcolor[0]);
	printf("floor color g: %d\n",dat->fcolor[1]);
	printf("floor color b: %d\n",dat->fcolor[2]);
	printf("ceiling color r: %d\n",dat->ccolor[0]);
	printf("ceiling color g: %d\n",dat->ccolor[1]);
	printf("ceiling color b: %d\n",dat->ccolor[2]);
	printf("heading : %f\n",dat->heading);
	//printf("x dir: %d\n",dat->xdir);
	//printf("y dir: %d\n",dat->ydir);
	printf("player pos x: %d\n",dat->xpos);
	printf("player pos y: %d\n",dat->ypos);
	printf("The map:\n");
	print_dblarr(dat->map);
}
