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

int	free_maplst(t_list **lst)
{
	t_list	*temp;

	temp = *lst;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		free(*lst);
		*lst = temp;
	}
	free(*lst);
	*lst = NULL;
	return (-1);
}

void	free_str(char **str)
{
	free(*str);
	*str = NULL;
}

int	free_return1(t_list **lst, char **str)
{
	free_maplst(lst);
	free_str(str);
	return (-1);
}

char	*free_strarr2(char **ptr, int len)
{
	int	i;

	i = 0;
	while (ptr && i < len)
	{
		//printf("printf free ptr %s\n",ptr[i]);
		if (ptr[i])
			free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	if (ptr)
		free(ptr);
	ptr = NULL;
	return (NULL);
}

void	free_strarr(char **ptr)
{
	int	i;

	i = -1;
	while (ptr && ptr[++i])
	{
		//printf("printf free ptr %s\n",ptr[i]);
		if (ptr[i])
			free(ptr[i]);
		ptr[i] = NULL;
	}
	if (ptr)
		free(ptr);
	ptr = NULL;
}
