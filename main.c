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

/*
char map[MHEIGHT][MWIDTH] = {
"11111111111",
"10000000001",
"10000110001",
"10000D10001",
"10000110001",
"10000000001",
"10000000001",
"11111111111",
};
*/

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
	params.map = dat->map;
	params.mwidth = dat->mwidth; 
	params.mheight = dat->mheight; 
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

	player.position[0] = dat->ypos + 0.5;
	player.position[1] = dat->xpos + 0.5;
	player.heading = dat->heading;
	printf("heading: %f\n", dat->heading);
	player.height = 0.5; // 10
	player.vert_angle = 0.0;
	player.speed = 1.0;
	player.god = false;
	player.move_ad = 0;
	player.move_ws = 0;
	player.move_tilt = 0;
	player.move_turn = 0;


	/* -------------------------------------------------------------------------- */
	/*                                TEXTURE INIT                                */
	/* -------------------------------------------------------------------------- */

	int width;
	int height;
	params.inner = mlx_xpm_file_to_image(mlx.ptr, "./incs/hallway.xpm", &width, &height); // extra
	params.spray = mlx_xpm_file_to_image(mlx.ptr, "./incs/mpivet-p.xpm", &width, &height); // extra
	params.door = mlx_xpm_file_to_image(mlx.ptr, "./incs/tunnelv2.xpm", &width, &height);
	params.north = mlx_xpm_file_to_image(mlx.ptr, dat->nxpm, &width, &height);
	params.south = mlx_xpm_file_to_image(mlx.ptr, dat->sxpm, &width, &height);
	params.east = mlx_xpm_file_to_image(mlx.ptr, dat->expm, &width, &height);
	params.west = mlx_xpm_file_to_image(mlx.ptr, dat->wxpm, &width, &height);

	params.fclr = dat->fclr;
	params.cclr = dat->cclr;
	/* -------------------------------------------------------------------------- */
	/*                              MLX HOOK AND LOOP                             */
	/* -------------------------------------------------------------------------- */


	mlx_do_key_autorepeatoff(mlx.ptr); // does this do anything?

	mlx_hook(mlx.win, ButtonPress, ButtonPressMask, &mouse_click, &params);
	mlx_hook(mlx.win, KeyRelease, KeyReleaseMask, &key_release_hook, &params);

	mlx_hook(mlx.win, MotionNotify, Button2MotionMask, &mouse_move, &params);
	mlx_hook(mlx.win, KeyPress, KeyPressMask, &key_hook, (void *) &params);
	mlx_hook(mlx.win, DestroyNotify, 0L, &close_window, (void *) &params);

	render(&params);
	mlx_loop_hook(mlx.ptr, render, &params);
	mlx_loop(mlx.ptr);
}

void draw_walls(t_params *p);

void draw_minimap(t_params p)
{
	int sq_size = 15;
	int mm_size = 8;
	int total_size = sq_size * mm_size;


	// start at center of minimap, draw straight line in direction of heading
	double heading = p.player->heading; 

	int pos_y = p.player->position[0];
	int pos_x = p.player->position[1];

	int off_y =(p.player->position[0] - pos_y) * sq_size;
	int off_x =(p.player->position[1] - pos_x) * sq_size;
	// printf("off y: %i off_x : %i\n", off_y, off_x);
	double half_projection_plane_width = tan(p.fov / 2.0);

	double heading_x = sin(heading);
	double heading_y = -cos(heading);
	double plane_x = -heading_y * half_projection_plane_width;
	double plane_y = heading_x * half_projection_plane_width;

	for (int px_col = 0; px_col < total_size; px_col++)
		for (int px_row = 0; px_row < total_size; px_row++)
				put_pixel(p, px_row, px_col, 0xa9a9a9);

	for (int ray = -WIN_WIDTH / 2; ray < WIN_WIDTH / 2; ray++)
	{

		double projection_plane_x = 2.0 * (double) ray / (WIN_WIDTH - 1); // goes from -1 to 1.
		double dir_x = heading_x + projection_plane_x * plane_x;
		double dir_y = heading_y + projection_plane_x * plane_y;
		double heading_px_row = total_size / 2;
		double heading_px_col = total_size / 2;
		while (heading_px_col < total_size && heading_px_col > 0
		&& heading_px_row > 0 && heading_px_row < total_size)
		{
			int col_check = pos_x - mm_size / 2 + (heading_px_col + off_x) / sq_size;
			int row_check = pos_y - mm_size / 2 + (heading_px_row + off_y) / sq_size;
			char block = p.map[row_check][col_check];
			if (col_check < 0 || row_check < 0 || col_check >= p.mwidth || row_check >= p.mheight)
				break ;
			if (block != '0' && block != 'd' && p.player->height <= 1.1 && p.player->height >= -0.1)
				break ;
			put_pixel(p, heading_px_row, heading_px_col, 0x550077);
			heading_px_row += dir_y;
			heading_px_col += dir_x;
		}
	}

	for (int px_col = 0; px_col < total_size; px_col++)
	{
		for (int px_row = 0; px_row < total_size; px_row++)
		{
			if (px_row == 0 || px_row == total_size -1 || px_col == 0 | px_col == total_size - 1)
				put_pixel(p, px_row, px_col, 0x000000);
			else if ((px_row + off_y) % sq_size == 0|| (px_col + off_x ) % sq_size == 0)
				put_pixel(p, px_row, px_col, 0xffffff);
			
			int col_check = pos_x - mm_size / 2 + (px_col + off_x) / sq_size;
			int row_check = pos_y - mm_size / 2 + (px_row + off_y) / sq_size;
			char block = p.map[row_check][col_check];
			// if (col_check < 0 || row_check < 0 || col_check >= MWIDTH || row_check >= MHEIGHT)
			if (col_check < 0 || row_check < 0 || col_check >= p.mwidth || row_check >= p.mheight)
				put_pixel(p, px_row, px_col, 0x111111);
			else if (block == 'a')
				put_pixel(p, px_row,px_col, 0x880000);
			else if (block != '0' && block != 'd')
				put_pixel(p, px_row,px_col, 0xff0000);
			//printf("rowcheck: %i, colcheck: %i, mwidth: %i mheight: %i\n", row_check, col_check, p.mwidth, p.mheight);
			if (px_col > total_size / 2 - 2 && px_col < total_size / 2 + 2
				&& px_row > total_size / 2 - 2 && px_row < total_size / 2 + 2)
					put_pixel(p, px_row, px_col, 0x00ffff);
		}
	}
}

void look_up_down(t_params *params, int direction)
{
	if (direction > 0 && params->player->vert_angle < M_PI / 6)
		params->player->vert_angle += M_PI / 120;
	else if (direction < 0 && params->player->vert_angle > -M_PI / 6)
		params->player->vert_angle -= M_PI / 120;

}

void acc_player(t_params *params)
{
	if (params->player->move_ws < 2 && params->player->move_ws > -2)
		params->player->move_ws *= 1.01;
	if (params->player->move_ad < 2 && params->player->move_ad > -2)
		params->player->move_ad *= 1.01;
	if (params->player->move_tilt < 2 && params->player->move_tilt > -2)
		params->player->move_tilt *= 1.01;
	if (params->player->move_turn < 2 && params->player->move_turn > -2)
		params->player->move_turn *= 1.01;

	
}
void move(t_params *params)
{
	acc_player(params);
	move_player(params, params->player->move_ws);
	strafe_player(params, params->player->move_ad);
	rotate_player(params, params->player->move_turn);
	look_up_down(params, params->player->move_tilt);

}

int render(t_params *p)
{
	// draw_grid(*p);
	// draw_player(*p);
	move(p);
	draw_walls(p);
	draw_crosshair(*p);
	draw_minimap(*p);
	mlx_put_image_to_window(p->mlx->ptr, p->mlx->win, p->mlx->img, 0, 0);
	return (1);
}
