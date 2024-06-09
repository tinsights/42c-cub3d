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
{2,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
{2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2},
{2,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,2},
{2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,2},
{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
};


int mouse_click(int button, int x, int y, t_params *params)
{
	params->clicked_px[0] = x;
	params->clicked_px[1] = y;
	printf("button: %i | mouseX: %i | mouseY: %i\n", button, x, y);
	return (1);
}

int mouse_move(int x, int y, t_params *params)
{
	extern double FOV;
	extern double playerVertAngle;

	int xDelta = x - params->clicked_px[0];
	int yDelta = y - params->clicked_px[1];

	// double projection_plane_width = 2.0 * tan(FOV / 2.0);

	//2∗atan(tan(h/2)∗AR)
	// printf("FOV: %f\n", FOV);

	double ratio = tan(FOV / 2) * 1.6;
	printf("ratio: %f\n", ratio);
	double vertFov = 2.0 * atan(ratio);
	printf("vertFov: %f\n", vertFov);

	double headingInc = (double) xDelta / WIN_WIDTH * tan(FOV / 2.0);
	double vertAngInc = (double) yDelta / WIN_HEIGHT * tan(vertFov / 2.0);
	params->player->heading += 4.0 * 1.6 * headingInc;

	playerVertAngle -= 4.0 * vertAngInc;
	// printf(" mouseX: %i | mouseY: %i | xDelta: %i | ydelta: %i | headingInc: %f vertAngInc: %f\n",  x, y, xDelta, yDelta, headingInc, vertAngInc);
	params->clicked_px[0] = x;
	params->clicked_px[1] = y;

	return (1);
}

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

	// put player on screen, curr centered in middle
	t_player player;

	params.player = &player;

	player.position[0] = 3.0;
	player.position[1] = 3.5;

	player.heading = 0;

	/* -------------------------------------------------------------------------- */
	/*                              MLX HOOK AND LOOP                             */
	/* -------------------------------------------------------------------------- */
	mlx_do_key_autorepeaton(mlx.ptr);
	mlx_hook(mlx.win, ButtonPress, ButtonPressMask, &mouse_click, &params);
	mlx_hook(mlx.win, MotionNotify, Button2MotionMask, &mouse_move, &params);
	// mlx_key_hook(mlx.win, &key_hook, (void *) &params);
	mlx_hook(mlx.win, KeyPress, KeyPressMask, &key_hook, (void *) &params);
	mlx_hook(mlx.win, DestroyNotify, 0L, &close_window, (void *) &params);
	mlx_loop_hook(mlx.ptr, render, &params);
	mlx_loop(mlx.ptr);
}
void draw_ray(t_params *p);


void draw_crosshair(t_params *p)
{
	int vertCenter = WIN_HEIGHT / 2;
	int horizCenter = WIN_WIDTH / 2;

	int halfCrosshairLength = 8;

	for (int i = vertCenter - halfCrosshairLength; i < vertCenter + halfCrosshairLength; i++)
		put_pixel(*p, i, horizCenter, 0x888800);
	for (int i = horizCenter - halfCrosshairLength; i < horizCenter + halfCrosshairLength; i++)
		put_pixel(*p, vertCenter, i, 0x888800);

	
}

int render(t_params *p)
{
	// draw_grid(*p);

	// draw one ray
	draw_ray(p);
	// draw_player(*p);

	draw_crosshair(p);

	mlx_put_image_to_window(p->mlx->ptr, p->mlx->win, p->mlx->img, 0, 0);
	return (1);
}
