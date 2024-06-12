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

int map[MHEIGHT][MWIDTH] = {
{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
{2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
{2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2},
{2,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,2},
{2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,2},
{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
};

int main(void)
{

	t_params params;
	/* -------------------------------------------------------------------------- */
	/*                                  MLX INIT                                  */
	/* -------------------------------------------------------------------------- */
	t_mlx mlx;
	params.mlx = &mlx;
	mlx.ptr = mlx_init();
	mlx.win= mlx_new_window(mlx.ptr, WIN_WIDTH, WIN_HEIGHT, "cub3d");
	mlx.img = mlx_new_image(mlx.ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx.ptr || !mlx.win || !mlx.img)
		ft_putstr_fd("Error initialising mlx\n", 2);

	mlx.img_addr = mlx_get_data_addr(mlx.img, &mlx.bpp, &mlx.line_sz, &mlx.endian);

	/* -------------------------------------------------------------------------- */
	/*                           PLAYER AND PARAMS INIT                           */
	/* -------------------------------------------------------------------------- */
	t_player player;

	params.player = &player;
	params.fov = FOV / 180.0 * M_PI;
	
	player.position[0] = 3.0;
	player.position[1] = 3.5;
	player.heading = 0;
	player.height = 0.5;
	player.vert_angle = 0.0;
	player.speed = 1.0;


	/* -------------------------------------------------------------------------- */
	/*                                TEXTURE INIT                                */
	/* -------------------------------------------------------------------------- */

	int width;
	int height;
	params.inner = mlx_xpm_file_to_image(mlx.ptr, "mpivet-p.xpm", &width, &height);
	params.north = mlx_xpm_file_to_image(mlx.ptr, "cooi.xpm", &width, &height);
	params.south = mlx_xpm_file_to_image(mlx.ptr, "jerlim.xpm", &width, &height);
	params.east = mlx_xpm_file_to_image(mlx.ptr, "achak.xpm", &width, &height);
	params.west = mlx_xpm_file_to_image(mlx.ptr, "so-leary.xpm", &width, &height);
	/* -------------------------------------------------------------------------- */
	/*                              MLX HOOK AND LOOP                             */
	/* -------------------------------------------------------------------------- */
	
	mlx_hook(mlx.win, ButtonPress, ButtonPressMask, &mouse_click, &params);
	// mlx_hook(mlx.win, KeyRelease, KeyReleaseMask, &key_release_hook, &params);

	mlx_hook(mlx.win, MotionNotify, Button2MotionMask, &mouse_move, &params);
	mlx_hook(mlx.win, KeyPress, KeyPressMask, &key_hook, (void *) &params);
	mlx_hook(mlx.win, DestroyNotify, 0L, &close_window, (void *) &params);
	mlx_do_key_autorepeaton(mlx.ptr); // does this do anything?

	render(&params);
	mlx_loop_hook(mlx.ptr, render, &params);
	mlx_loop(mlx.ptr);
}

void draw_walls(t_params *p);

int render(t_params *p)
{
	// draw_grid(*p);
	// draw_player(*p);

	draw_walls(p);
	draw_crosshair(*p);

	mlx_put_image_to_window(p->mlx->ptr, p->mlx->win, p->mlx->img, 0, 0);
	return (1);
}
