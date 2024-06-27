/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 20:25:20 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/13 20:25:24 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	nondigits(char *str)
{
	int	comma;

	comma = 0;
	while (*str)
	{
		if (*str == ',')
			comma++;
		else if (!ft_isdigit(*str))
			return (1);
		if (comma > 2)
			return (1);
		str++;
	}
	return (0);
}

int	validchr(char c, char *vchr)
{
	while (*vchr)
	{
		if (c == *vchr++)
			return (1);
	}
	return (0);
}

int	validstr(char *str, char *vchr)
{
	while (*str)
	{
		if (!validchr(*str++, vchr))
			return (0);
	}
	return (1);
}

int	wcount(char **elem)
{
	int	i;

	i = 0;
	while (elem[i])
		i++;
	return (i);
}
