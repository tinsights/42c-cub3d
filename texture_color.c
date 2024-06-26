/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sthiagar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 19:33:02 by sthiagar          #+#    #+#             */
/*   Updated: 2024/06/24 19:33:14 by sthiagar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	isvalid_typeid(char *elem, char *target)
{
	int	len;

	len = ft_strlen(elem);
	if (len == 1 && elem[0] == target[0])
		return (1);
	else if (len == 2 && elem[0] == target[0] && elem[1] == target[1])
		return (1);
	return (0);
}

int	update_color(int *color, char *colorstr)
{
	char	**rgb;
	int		irgb[3];
	int		i;

	if (nondigits(colorstr))
		return (-1);
	rgb = ft_split(colorstr, ',');
	if (wcount(rgb) != 3)
		return (-1);
	i = 0;
	while (i < 3)
	{
		irgb[i] = ft_atoi(rgb[i]);
		if (irgb[i] < 0 || irgb[i] > 255)
			break ;
		i++;
	}
	free_strarr(rgb);
	if (i < 3)
		return (-1);
	*color = (irgb[0] << 16) | irgb[1] << 8 | irgb[2];
	return (1);
}

int	update_filepath(char **filepath, char *pathstr)
{
	t_mlx	*mlx;
	void	*image;
	int		tmp;

	if (*filepath != NULL)
		return (-1);
	mlx = mlx_init();
	if (!mlx)
		return (-1);
	image = mlx_xpm_file_to_image(mlx, pathstr, &tmp, &tmp);
	if (image == NULL)
	{
		mlx_destroy_display(mlx);
		free(mlx);
		return (-1);
	}
	mlx_destroy_image(mlx, image);
	mlx_destroy_display(mlx);
	free(mlx);
	*filepath = ft_strdup(pathstr);
	return (1);
}

int	update_path_color(char **elem, t_input *dat)
{
	if (isvalid_typeid(elem[0], "NO"))
		return (update_filepath(&dat->nxpm, elem[1]));
	else if (isvalid_typeid(elem[0], "SO"))
		return (update_filepath(&dat->sxpm, elem[1]));
	else if (isvalid_typeid(elem[0], "WE"))
		return (update_filepath(&dat->wxpm, elem[1]));
	else if (isvalid_typeid(elem[0], "EA"))
		return (update_filepath(&dat->expm, elem[1]));
	else if (isvalid_typeid(elem[0], "F"))
		return (update_color(&dat->fclr, elem[1]));
	else if (isvalid_typeid(elem[0], "C"))
		return (update_color(&dat->cclr, elem[1]));
	return (-1);
}

int	validate_path_color(char *line, int *vcount, t_input *dat)
{
	char	**elem;

	elem = ft_split2(line, " \n");
	if (elem == NULL)
		return (-1);
	if (!(wcount(elem) == 0 || wcount(elem) == 2))
	{
		free_strarr(elem);
		return (-1);
	}
	else if (wcount(elem) == 2)
		*vcount += update_path_color(elem, dat);
	free_strarr(elem);
	return (1);
}

int	parse_path_color(int fd, t_input *dat)
{
	int		vcount;
	char	*line;

	vcount = 0;
	line = get_next_line(fd, 0);
	while (vcount < 6 && line)
	{
		if (validate_path_color(line, &vcount, dat) == -1)
		{
			get_next_line(fd, 1);
			break ;
		}
		free_str(&line);
		line = get_next_line(fd, 0);
	}
	free_str(&line);
	if (vcount < 6)
		return (-1);
	return (1);
}
