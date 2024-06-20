/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 20:17:21 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/13 20:17:30 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	chktypeid(char *elem, char *target)// N or NO, S OR SO etc
{
	int len;

	len = ft_strlen(elem);
	if (len == 1 && *elem == *target)
		return (1);
	else if (len == 2 && *elem == *target \
			&& *(elem++) == *(target++))
		return (1);
	return (0);
}

int	update_xpmpath(char **xpm, char *elem)//valid xpm file path
{
	t_mlx	*mlx;
	int	tmp;
	void	*image;

	if (*xpm != NULL)//*xpm != NULL means duplicate found
		return (-1);
	mlx = mlx_init();
	if (!mlx)
		return (-1);
	image = mlx_xpm_file_to_image(mlx, elem, &tmp, &tmp);
	if (image == NULL)
	{
		free(mlx);
		return (-1);
	}
	mlx_destroy_image(mlx, image);
	free(mlx);
	*xpm = ft_strdup(elem);
	return (1);
}

int	update_color(int (*color)[3], char *elem) //pass only elem[1]?
{
	char	**rgb;
	int	i;

	if (nondigits(elem))
		return (-1);
	rgb = ft_split(elem, ',');
	if (wcount(rgb) != 3)
		return (-1);
	i = 0;
	while (i < 3)
	{
		(*color)[i] = ft_atoi(rgb[i]);
		if ((*color)[i] < 0 || (*color)[i] > 255)
		{
			free_strarr(rgb);
			return (-1);
		}
		i++;
	}
	free_strarr(rgb);
	return (1);
}

int	is_valid_typeid(char **elem, t_input *dat)
{
	if (chktypeid(elem[0], "NO"))
		return (update_xpmpath(&dat->nxpm, elem[1]));
	else if (chktypeid(elem[0], "SO"))
		return (update_xpmpath(&dat->sxpm, elem[1]));
	else if (chktypeid(elem[0], "WE"))
		return (update_xpmpath(&dat->wxpm, elem[1]));
	else if (chktypeid(elem[0], "EA"))
		return (update_xpmpath(&dat->expm, elem[1]));
	else if (chktypeid(elem[0], "F"))
		return (update_color(&dat->fcolor, elem[1]));
	else if (chktypeid(elem[0], "C"))
		return (update_color(&dat->ccolor, elem[1]));
	return (-1);
}

int 	validate_parseline(char *line, int *vcount, t_input *dat)
{
	char 	**elem;

	elem = ft_split2(line, " \n");//delimiter space, \n
	if (elem == NULL)
		return (-1);
	if (wcount(elem) == 1 || wcount(elem) > 2)
		return (-1);
	else if (wcount(elem) == 2)//wcount == 0 no action
		*vcount += is_valid_typeid(elem, dat);
	free_strarr(elem);
	return (1);
}

int	validate_typeid(int fd, t_input *dat)
{
	int	vcount;
	char	*line;

	vcount = 0;
	line = get_next_line(fd);
	while(vcount < 6 && line)
	{
		if (validate_parseline(line, &vcount, dat) == -1)
			return (-1);
		free (line);
		line = get_next_line(fd);
	}
	free (line);
	if (vcount < 6)
		return (-1);
	//update fclr and cclr; R,G,B
	dat->fclr = (dat->fcolor[0] << 16) | dat->fcolor[1] << 8 | dat->fcolor[2];
	dat->cclr = (dat->ccolor[0] << 16) | dat->ccolor[1] << 8 | dat->ccolor[2];
	
	return(1);
}
