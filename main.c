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

int	main(int argc, char *argv[])
{
	t_input		*dat;
	t_params	params;
	t_mlx		mlx;
	t_player	player;
	int			width;
	int			height;

	dat = (t_input *)malloc(sizeof(t_input));
	if (dat == NULL)
		return (0);
	if (argc != 2)
		return (0);
	else
		get_data(argv[1], dat);
	/*To test valgrind t_input *dat is freed correctly*/
	// free_strarr(dat->map);
	// free_xpmpath(dat); //frees dat also
	// return(0);
	/*---------------------------------------------------*/
	params.map = dat->map;
	params.mwidth = dat->mwidth;
	params.mheight = dat->mheight;
	/* -------------------------------------------------------------------------- */
	/*                                  MLX INIT                                  */
	/* -------------------------------------------------------------------------- */
	params.mlx = &mlx;
	mlx.ptr = mlx_init();
	mlx.win = mlx_new_window(mlx.ptr, WIN_WIDTH, WIN_HEIGHT, "cub3d");
	mlx.img = mlx_new_image(mlx.ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx.ptr || !mlx.win || !mlx.img)
		ft_putstr_fd("Error initialising mlx\n", 2);
	mlx.img_addr = mlx_get_data_addr(mlx.img, &mlx.bpp, &mlx.line_sz,
			&mlx.endian);
	/* -------------------------------------------------------------------------- */
	/*                           PLAYER AND PARAMS INIT                           */
	/* -------------------------------------------------------------------------- */
	params.player = &player;
	params.fov = FOV / 180.0 * M_PI;
	params.lights = false;
	player.position[0] = dat->ypos + 0.5;
	player.position[1] = dat->xpos + 0.5;
	player.heading = dat->heading;
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
	params.inner = mlx_xpm_file_to_image(mlx.ptr, "./incs/hallway.xpm", &width,
			&height); // extra
	params.spray = mlx_xpm_file_to_image(mlx.ptr, "./incs/42sg.xpm", &width,
			&height);    // extra
	params.door = mlx_xpm_file_to_image(mlx.ptr, "./incs/tunnelv2.xpm", &width,
			&height);
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
	mlx_hook(mlx.win, KeyPress, KeyPressMask, &key_hook, (void *)&params);
	mlx_hook(mlx.win, DestroyNotify, 0L, &close_window, (void *)&params);
	render(&params);
	mlx_loop_hook(mlx.ptr, render, &params);
	mlx_loop(mlx.ptr);
}

void	draw_walls(t_params *p);


typedef struct s_minimap {
	int		sq_size;
	int		mm_size;
	int		total_size;
	double	heading;
	int		pos_y;
	int		pos_x;
	int		off_y;
	int		off_x;
	double	half_projection_plane_width;
	double	heading_x;
	double	heading_y;
	double	plane_x;
	double	plane_y;
	double	dir_x;
	double	dir_y;
	double	heading_px_row;
	double	heading_px_col;
	int		col_check;
	int		row_check;
	char	block;
} t_minimap;

void draw_rays(t_params *p, t_minimap mm)
{
	for (int ray = -WIN_WIDTH / 2; ray < WIN_WIDTH / 2; ray++)
	{
		double projection_plane_x = 2.0 * (double)ray / (WIN_WIDTH - 1);
		mm.dir_x = mm.heading_x + projection_plane_x * mm.plane_x;
		mm.dir_y = mm.heading_y + projection_plane_x * mm.plane_y;
		mm.heading_px_row = mm.total_size / 2;
		mm.heading_px_col = mm.total_size / 2;
		while (mm.heading_px_col < mm.total_size && mm.heading_px_col > 0
			&& mm.heading_px_row > 0 && mm.heading_px_row < mm.total_size)
		{
			mm.col_check = mm.pos_x - mm.mm_size / 2 + (mm.heading_px_col + mm.off_x)
				/ mm.sq_size;
			mm.row_check = mm.pos_y - mm.mm_size / 2 + (mm.heading_px_row + mm.off_y)
				/ mm.sq_size;
			if (mm.col_check < 0 || mm.row_check < 0 || mm.col_check >= p->mwidth
				|| mm.row_check >= p->mheight)
				break ;
			mm.block = p->map[mm.row_check][mm.col_check];
			if (mm.block != '0' && mm.block != 'd' && (p->lights
					|| (p->player->height <= 1.1 && p->player->height >= -0.1)))
				break ;
			put_pixel(*p, mm.heading_px_row, mm.heading_px_col, 0x550077);
			mm.heading_px_row += mm.dir_y;
			mm.heading_px_col += mm.dir_x;
		}
	}
}
void fill_grid(t_params *p, t_minimap mm)
{
	for (int px_col = 0; px_col < mm.total_size; px_col++)
	{
		for (int px_row = 0; px_row < mm.total_size; px_row++)
		{
			if (px_row == 0 || px_row == mm.total_size - 1
				|| px_col == 0 | px_col == mm.total_size - 1)
				put_pixel(*p, px_row, px_col, 0x000000);
			else if ((px_row + mm.off_y) % mm.sq_size == 0 || (px_col + mm.off_x)
				% mm.sq_size == 0)
				put_pixel(*p, px_row, px_col, 0xffffff);
			mm.col_check = mm.pos_x - mm.mm_size / 2 + (px_col + mm.off_x) / mm.sq_size;
			mm.row_check = mm.pos_y - mm.mm_size / 2 + (px_row + mm.off_y) / mm.sq_size;
			if (mm.col_check < 0 || mm.row_check < 0 || mm.col_check >= p->mwidth
				|| mm.row_check >= p->mheight)
			{
				put_pixel(*p, px_row, px_col, 0x111111);
				continue ;
			}
			mm.block = p->map[mm.row_check][mm.col_check];
			if (mm.block == 'a')
				put_pixel(*p, px_row, px_col, 0x880000);
			if (mm.block != '0' && mm.block != 'd')
				put_pixel(*p, px_row, px_col, 0xff0000);
			if (px_col > mm.total_size / 2 - 2 && px_col < mm.total_size / 2 + 2
				&& px_row > mm.total_size / 2 - 2 && px_row < mm.total_size / 2 + 2)
				put_pixel(*p, px_row, px_col, 0x00ffff);
		}
	}
}

void	draw_minimap(t_params *p)
{
	t_minimap mm;
	mm.sq_size = 15;
	mm.mm_size = 8;
	mm.total_size = mm.sq_size * mm.mm_size;
	
	mm.heading = p->player->heading;
	mm.pos_y = p->player->position[0];
	mm.pos_x = p->player->position[1];
	mm.off_y = (p->player->position[0] - mm.pos_y) * mm.sq_size;
	mm.off_x = (p->player->position[1] - mm.pos_x) * mm.sq_size;
	// printf("off y: %i off_x : %i\n", off_y, off_x);
	mm.half_projection_plane_width = tan(p->fov / 2.0);
	mm.heading_x = sin(mm.heading);
	mm.heading_y = -cos(mm.heading);
	mm.plane_x = -mm.heading_y * mm.half_projection_plane_width;
	mm.plane_y = mm.heading_x * mm.half_projection_plane_width;
	for (int px_col = 0; px_col < mm.total_size; px_col++)
		for (int px_row = 0; px_row < mm.total_size; px_row++)
			put_pixel(*p, px_row, px_col, 0xa9a9a9);
	draw_rays(p, mm);
	fill_grid(p, mm);
	
}

void	look_up_down(t_params *params, int direction)
{
	if (direction > 0 && params->player->vert_angle < M_PI / 6)
		params->player->vert_angle += M_PI / 180;
	else if (direction < 0 && params->player->vert_angle > -M_PI / 6)
		params->player->vert_angle -= M_PI / 180;
}

void	move(t_params *params)
{
	move_player(params, params->player->move_ws);
	strafe_player(params, params->player->move_ad);
	rotate_player(params, params->player->move_turn);
	look_up_down(params, params->player->move_tilt);
}

int	render(t_params *p)
{
	// draw_grid(*p);
	// draw_player(*p);
	move(p);
	draw_walls(p);
	draw_crosshair(*p);
	draw_minimap(p);
	mlx_put_image_to_window(p->mlx->ptr, p->mlx->win, p->mlx->img, 0, 0);
	return (1);
}
