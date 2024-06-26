/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 20:30:19 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/13 20:30:25 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_xpmpath(t_input *dat)
{
	if (dat->nxpm != NULL)
		free(dat->nxpm);
	if (dat->sxpm != NULL)
		free(dat->sxpm);
	if (dat->expm != NULL)
		free(dat->expm);
	if (dat->wxpm != NULL)
		free(dat->wxpm);
	dat->nxpm = NULL;
	dat->sxpm = NULL;
	dat->expm = NULL;
	dat->wxpm = NULL;
	if (dat)
		free(dat);
	dat = NULL;
}

void	free_intarr(int **arr, int size)
{
	int	j;

	j = 0;
	while (j < size)
	{
		free(arr[j]);
		j++;
	}
	free(arr);
	arr = NULL;
}

void	free_maplst(t_list **lst)
{
	t_list	*temp;

	temp = *lst;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		if ((*lst)->content)
			free((*lst)->content);
		free(*lst);
		*lst = temp;
	}
	free(*lst);
	*lst = NULL;
}

void	free_str(char **str)
{
	free(*str);
	*str = NULL;
}

void	free_strarr(char **ptr)
{
	int	i;

	i = -1;
	while (ptr && ptr[++i])
	{
		if (ptr[i])
			free(ptr[i]);
		ptr[i] = NULL;
	}
	if (ptr)
		free(ptr);
	ptr = NULL;
}
