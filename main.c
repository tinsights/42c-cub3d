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


char map[MHEIGHT][MWIDTH] = {
"11111111111",
"10000000001",
"10000000001",
"10000110001",
"10000110001",
"10000000001",
"10000000001",
"11111111111",
};

// int map[MHEIGHT][MWIDTH] = {
// {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
// {2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
// {2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
// {2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2},
// {2,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,2},
// {2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,2},
// {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
// {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
// };

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
	
	player.position[0] = 3.5;
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
	params.spray = mlx_xpm_file_to_image(mlx.ptr, "mpivet-p.xpm", &width, &height);
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
	// mlx_loop_hook(mlx.ptr, render, &params);
	mlx_loop(mlx.ptr);
}

void draw_walls(t_params *p);

void draw_minimap(t_params p)
{
	int sq_size = 20;
	int mm_size = 5;
	int total_size = sq_size * mm_size;

	int pos_y = p.player->position[0];
	int pos_x = p.player->position[1];

	int off_y = sq_size / 2 + (p.player->position[0] - pos_y) * sq_size;
	int off_x = sq_size / 2 + (p.player->position[1] - pos_x) * sq_size;
	printf("off y: %i off_x : %i\n", off_y, off_x);
	for (int px_col = 0; px_col < total_size; px_col++)
	{
		for (int px_row = 0; px_row < total_size; px_row++)
		{
			if ((px_row + off_y) % sq_size == 0|| (px_col + off_x ) % sq_size == 0)
				put_pixel(p, px_row, px_col, 0xffffff);
			
			int col_check = pos_x - mm_size / 2 + (px_col + off_x) / sq_size - 1;
			int row_check = pos_y - mm_size / 2 + (px_row + off_y) / sq_size - 1;
			if (col_check < 0 || row_check < 0 || col_check >= MWIDTH || row_check >= MHEIGHT)
				put_pixel(p, px_row, px_col, 0x11111);
			if (map[row_check][col_check] == '1')
				put_pixel(p, px_row,px_col, 0xff0000);
			if (px_col > total_size / 2 - 2 && px_col < total_size / 2 + 2
				&& px_row > total_size / 2 - 2 && px_row < total_size / 2 + 2)
				{
					put_pixel(p, px_row, px_col, 0x00ffff);
				}
		}
	}

}

int render(t_params *p)
{
	// draw_grid(*p);
	// draw_player(*p);

	draw_walls(p);
	draw_crosshair(*p);
	draw_minimap(*p);
	mlx_put_image_to_window(p->mlx->ptr, p->mlx->win, p->mlx->img, 0, 0);
	return (1);
}
