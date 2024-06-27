/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:09:43 by tjegades          #+#    #+#             */
/*   Updated: 2024/05/21 13:09:45 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	close_window(t_params *params)
{
	mlx_do_key_autorepeaton(params->mlx.ptr);
	free_strarr(params->input.map);
	free(params->input.nxpm);
	free(params->input.sxpm);
	free(params->input.expm);
	free(params->input.wxpm);
	mlx_destroy_image(params->mlx.ptr, params->mlx.img);
	mlx_destroy_image(params->mlx.ptr, params->north);
	mlx_destroy_image(params->mlx.ptr, params->east);
	mlx_destroy_image(params->mlx.ptr, params->west);
	mlx_destroy_image(params->mlx.ptr, params->south);
	if (params->spray)
		mlx_destroy_image(params->mlx.ptr, params->spray);
	if (params->door)
		mlx_destroy_image(params->mlx.ptr, params->door);
	if (params->inner)
		mlx_destroy_image(params->mlx.ptr, params->inner);
	mlx_destroy_window(params->mlx.ptr, params->mlx.win);
	mlx_destroy_display(params->mlx.ptr);
	free(params->mlx.ptr);
	exit(1);
	return (1);
}

bool	explorable(t_params *params, double y, double x)
{
	if (x < 0 || x > params->mwidth || y < 0 || y > params->mheight)
		return (false);
	return (params->player.god || !is_wall(params->map[(int)y][(int)x]));
}
