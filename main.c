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

int main(int argc, char *argv[])
{
	t_input	*dat;

	dat = (t_input *)malloc(sizeof(t_input));
	if (dat == NULL)
		return (0);
	if (argc != 2)
		return (0);
	else
		get_data(argv[1], dat);
	t_params params;
	
	print_input(dat);
	printf("end of validation\n");
	return (0);
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
	/*                           Player and Params init                           */
	/* -------------------------------------------------------------------------- */
	t_player player;

	params.player = &player;
	params.fov = FOV / 180.0 * M_PI;
	
	player.position[0] = 3.0; // 300
	player.position[1] = 3.9; // 390
	player.heading = 0;
	player.height = 0.1; // 10
	player.vert_angle = 0.0;
	player.speed = 1.0;

	/* -------------------------------------------------------------------------- */
	/*                              MLX HOOK AND LOOP                             */
	/* -------------------------------------------------------------------------- */
	
	mlx_hook(mlx.win, ButtonPress, ButtonPressMask, &mouse_click, &params);
	// mlx_hook(mlx.win, KeyRelease, KeyReleaseMask, &key_release_hook, &params);

	mlx_hook(mlx.win, MotionNotify, Button2MotionMask, &mouse_move, &params);
	// mlx_key_hook(mlx.win, &key_hook, (void *) &params);
	mlx_hook(mlx.win, KeyPress, KeyPressMask, &key_hook, (void *) &params);
	mlx_hook(mlx.win, DestroyNotify, 0L, &close_window, (void *) &params);
	mlx_do_key_autorepeaton(mlx.ptr); // does this do anything?

	render(&params);
	// mlx_loop_hook(mlx.ptr, render, &params);
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
