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

int map[6][6] = {
{1,1,1,1,1,1},
{1,0,0,1,0,1},
{1,0,0,0,0,1},
{1,1,0,0,0,1},
{1,0,0,0,1,1},
{1,1,1,1,1,1},
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
	mlx.win= mlx_new_window(mlx.ptr, SIZE, SIZE, "squar2d");
	mlx.img = mlx_new_image(mlx.ptr, SIZE, SIZE);
	if (!mlx.ptr || !mlx.win || !mlx.img)
		ft_putstr_fd("Errorr initialising mlx\n", 2);

	mlx.img_addr = mlx_get_data_addr(mlx.img, &mlx.bpp, &mlx.line_sz, &mlx.endian);

	// put player on screen, curr centered in middle
	t_player player;

	params.player = &player;

	player.position[0] = 3.5;
	player.position[1] = 3.5;

	player.heading = 0;

	/* -------------------------------------------------------------------------- */
	/*                              MLX HOOK AND LOOP                             */
	/* -------------------------------------------------------------------------- */
	mlx_do_key_autorepeaton(mlx.ptr);
	mlx_hook(mlx.win, KeyPress, 1L, &key_hook, (void *) &params);
	mlx_hook(mlx.win, DestroyNotify, 0L, &close_window, (void *) &params);
	mlx_do_key_autorepeaton(mlx.ptr);
	mlx_loop_hook(mlx.ptr, render, &params);
	mlx_loop(mlx.ptr);
}
void draw_ray(t_params *p);


int render(t_params *p)
{
	draw_grid(*p);
	draw_player(*p);

	// draw one ray
	draw_ray(p);

	mlx_put_image_to_window(p->mlx->ptr, p->mlx->win, p->mlx->img, 0, 0);
	return (1);
}
