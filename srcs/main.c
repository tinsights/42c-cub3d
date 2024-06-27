/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:48:47 by tjegades          #+#    #+#             */
/*   Updated: 2024/05/06 12:49:17 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_data(char *mapfile, t_input	*dat);
void	init_params(t_params *params);
int		render(t_params *p);

int	main(int argc, char *argv[])
{
	t_params	params;

	if (argc != 2)
		return (0);
	get_data(argv[1], &(params.input));
	init_params(&params);
	mlx_loop(params.mlx.ptr);
}

void	init_player(t_params *params)
{
	params->player.position[0] = params->input.ypos + 0.5;
	params->player.position[1] = params->input.xpos + 0.5;
	params->player.heading = params->input.heading;
	params->player.height = 0.5;
	params->player.vert_angle = 0.0;
	params->player.speed = 1.0;
	params->player.god = false;
	params->player.move_ad = 0;
	params->player.move_ws = 0;
	params->player.move_tilt = 0;
	params->player.move_turn = 0;
}

void	init_mlx(t_params *params)
{
	params->mlx.ptr = mlx_init();
	params->mlx.win = mlx_new_window(params->mlx.ptr, WIN_WIDTH, WIN_HEIGHT,
			"cub3d");
	params->mlx.img = mlx_new_image(params->mlx.ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!params->mlx.ptr || !params->mlx.win || !params->mlx.img)
	{
		ft_putstr_fd("Error initialising mlx\n", 2);
		exit(EXIT_FAILURE);
	}
	params->mlx.img_addr = mlx_get_data_addr(params->mlx.img, &params->mlx.bpp,
			&params->mlx.line_sz, &params->mlx.endian);
	mlx_do_key_autorepeatoff(params->mlx.ptr);
}

void	get_textures(t_params *params)
{
	int	height;
	int	width;

	params->inner = mlx_xpm_file_to_image(params->mlx.ptr, "./incs/hallway.xpm",
			&width, &height);
	params->spray = mlx_xpm_file_to_image(params->mlx.ptr, "./incs/42sg.xpm",
			&width, &height);
	params->door = mlx_xpm_file_to_image(params->mlx.ptr, "./incs/tunnelv2.xpm",
			&width, &height);
	params->north = mlx_xpm_file_to_image(params->mlx.ptr, params->input.nxpm,
			&width, &height);
	params->south = mlx_xpm_file_to_image(params->mlx.ptr, params->input.sxpm,
			&width, &height);
	params->east = mlx_xpm_file_to_image(params->mlx.ptr, params->input.expm,
			&width, &height);
	params->west = mlx_xpm_file_to_image(params->mlx.ptr, params->input.wxpm,
			&width, &height);
	params->fclr = params->input.fclr;
	params->cclr = params->input.cclr;
}

void	init_params(t_params *params)
{
	params->map = params->input.map;
	params->mwidth = params->input.mwidth;
	params->mheight = params->input.mheight;
	params->fov = FOV / 180.0 * M_PI;
	params->lights = false;
	init_mlx(params);
	init_player(params);
	get_textures(params);
	mlx_hook(params->mlx.win, ButtonPress, ButtonPressMask, &mouse_click,
		params);
	mlx_hook(params->mlx.win, KeyRelease, KeyReleaseMask, &key_release_hook,
		params);
	mlx_hook(params->mlx.win, MotionNotify, Button2MotionMask, &mouse_move,
		params);
	mlx_hook(params->mlx.win, KeyPress, KeyPressMask, &key_hook, params);
	mlx_hook(params->mlx.win, DestroyNotify, 0L, &close_window, params);
	mlx_loop_hook(params->mlx.ptr, render, params);
}
