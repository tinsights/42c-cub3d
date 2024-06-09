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


int mouse_click(int button, int x, int y, t_params *params)
{
	printf("button: %i | mouseX: %i | mouseY: %i\n", button, x, y);

	if (button == 2)
	{
		params->clicked_px[0] = x;
		params->clicked_px[1] = y;
	}
	else if (button == 4)
	{
		printf("zooming in\n");
	}
	else if (button == 5)
	{
		printf("zooming out\n");
	}
	return (1);
}

int mouse_move(int x, int y, t_params *params)
{
	t_player *player = params->player;

	int xDelta = x - params->clicked_px[0];
	int yDelta = y - params->clicked_px[1];

	// double projection_plane_width = 2.0 * tan(params->fov / 2.0);

	//2∗atan(tan(h/2)∗AR)
	// printf("FOV: %f\n", params->fov);

	double ratio = tan(params->fov / 2) * 1.6;
	printf("ratio: %f\n", ratio);
	double vertFov = 2.0 * atan(ratio);
	printf("vertFov: %f\n", vertFov);

	double heading_delta = (double) xDelta / WIN_WIDTH * tan(params->fov / 2.0);
	double vert_delta = (double) yDelta / WIN_WIDTH * tan(params->fov / 2.0);
	params->player->heading += 4.0 * heading_delta;

	player->vert_angle -= 4.0 * vert_delta;
	// printf(" mouseX: %i | mouseY: %i | xDelta: %i | ydelta: %i | heading_delta: %f vert_delta: %f\n",  x, y, xDelta, yDelta, heading_delta, vert_delta);
	params->clicked_px[0] = x;
	params->clicked_px[1] = y;
	render(params);

	return (1);
}


int key_release_hook(int keycode, t_params *params)
{
	printf("keycode: %i\n", keycode);
	if (keycode == XK_w && params)
	{
		printf("hello\n");
	}
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


	params.fov = FOV / 180.0 * M_PI;
	// put player on screen, curr _centered in middle
	t_player player;

	params.player = &player;

	player.position[0] = 3.0;
	player.position[1] = 3.5;

	player.heading = 0;

	player.height = 0.5;
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


void draw_crosshair(t_params p)
{
	int vert_center = WIN_HEIGHT / 2;
	int horiz_center = WIN_WIDTH / 2;

	int half_crosshair_length = 8;

	for (int i = vert_center - half_crosshair_length; i < vert_center + half_crosshair_length; i++)
		put_pixel(p, i, horiz_center, 0x888800);
	for (int i = horiz_center - half_crosshair_length; i < horiz_center + half_crosshair_length; i++)
		put_pixel(p, vert_center, i, 0x888800);
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
