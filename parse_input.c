/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 20:05:57 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/13 20:06:02 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	error_exit(int fd, char *str, int p_err)
{
	write(1, "Error", 5);
	write(1, "\n", 1);
	if (fd > -1)
		close(fd);
	if (p_err)
		perror(str);
	else
	{
		write(1, str, ft_strlen(str));
		write(1, "\n", 1);
	}
	exit(EXIT_FAILURE);
}

void	init_input(t_input *dat)
{
	dat->nxpm = NULL;
	dat->sxpm = NULL;
	dat->expm = NULL;
	dat->wxpm = NULL;
	dat->fcolor[0] = -1;
	dat->fcolor[1] = -1;
	dat->fcolor[2] = -1;
	dat->ccolor[0] = -1;
	dat->ccolor[1] = -1;
	dat->ccolor[2] = -1;
	dat->fclr = 0;
	dat->cclr = 0;
	dat->fov = 0;
	dat->xdir = 0;
	dat->ydir = 0;
	dat->xpos = 0;
	dat->ypos = 0;
	dat->map = NULL;
}

void 	get_data(char *mapfile, t_input	*dat)
{
	int	fd;
	
	init_input(dat);
	fd = open(mapfile, O_RDONLY);
	if (fd == -1)
		error_exit(-1, "Open", 1);
	if (validate_typeid(fd, dat) == -1)
		error_exit(fd, "Invalid Typeid", 0);//closed (fd)
	if (parse_map(fd, dat) == -1)
		error_exit(fd, "Invalid Map", 0);//closed (fd)
	close(fd);
}
