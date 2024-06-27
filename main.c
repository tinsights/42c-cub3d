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
}

int	main(int argc, char *argv[])
{
	t_params	params;

	if (argc != 2)
		return (0);
	get_data(argv[1], &(params.input));
	init_params(&params);
	mlx_do_key_autorepeatoff(params.mlx.ptr);
	mlx_hook(params.mlx.win, ButtonPress, ButtonPressMask, &mouse_click,
		&params);
	mlx_hook(params.mlx.win, KeyRelease, KeyReleaseMask, &key_release_hook,
		&params);
	mlx_hook(params.mlx.win, MotionNotify, Button2MotionMask, &mouse_move,
		&params);
	mlx_hook(params.mlx.win, KeyPress, KeyPressMask, &key_hook,
		(void *)&params);
	mlx_hook(params.mlx.win, DestroyNotify, 0L, &close_window, (void *)&params);
	render(&params);
	mlx_loop_hook(params.mlx.ptr, render, &params);
	mlx_loop(params.mlx.ptr);
}

void	draw_walls(t_params *p);

bool	hit_wall(t_params *p, t_minimap *mm)
{
	mm->col_check = mm->pos_x - mm->mm_size / 2 + (mm->heading_px_col
			+ mm->off_x) / mm->sq_size;
	mm->row_check = mm->pos_y - mm->mm_size / 2 + (mm->heading_px_row
			+ mm->off_y) / mm->sq_size;
	if (mm->col_check < 0 || mm->row_check < 0
		|| mm->col_check >= p->mwidth || mm->row_check >= p->mheight)
		return (true);
	mm->block = p->map[mm->row_check][mm->col_check];
	if (mm->block != '0' && mm->block != 'd' && (p->lights
			|| (p->player.height <= 1.1 && p->player.height >= -0.1)))
		return (true);
	return (false);
}

void	draw_rays(t_params *p, t_minimap *mm)
{
	double	projection_plane_x;
	int		ray;

	ray = -WIN_WIDTH / 2;
	while (ray < WIN_WIDTH / 2)
	{
		projection_plane_x = 2.0 * (double)ray / (WIN_WIDTH - 1);
		mm->dir_x = mm->heading_x + projection_plane_x * mm->plane_x;
		mm->dir_y = mm->heading_y + projection_plane_x * mm->plane_y;
		mm->heading_px_row = mm->total_size / 2;
		mm->heading_px_col = mm->total_size / 2;
		while (mm->heading_px_col < mm->total_size && mm->heading_px_col > 0
			&& mm->heading_px_row > 0 && mm->heading_px_row < mm->total_size)
		{
			if (hit_wall(p, mm))
				break ;
			put_pixel(p, mm->heading_px_row, mm->heading_px_col, 0x550077);
			mm->heading_px_row += mm->dir_y;
			mm->heading_px_col += mm->dir_x;
		}
		ray++;
	}
}

void	paint_player(t_params *p, t_minimap *mm)
{
	if (mm->px_col > mm->total_size / 2 - 2 && mm->px_col < mm->total_size / 2
		+ 2 && mm->px_row > mm->total_size / 2 - 2
		&& mm->px_row < mm->total_size / 2 + 2)
		put_pixel(p, mm->px_row, mm->px_col, 0x00ffff);
}

void	paint_block(t_params *p, t_minimap *mm)
{
	mm->block = p->map[mm->row_check][mm->col_check];
	if (mm->block == 'a')
		put_pixel(p, mm->px_row, mm->px_col, 0x880000);
	if (mm->block != '0' && mm->block != 'd')
		put_pixel(p, mm->px_row, mm->px_col, 0xff0000);
}

bool	unbounded(t_params *p, t_minimap *mm)
{
	mm->col_check = mm->pos_x - mm->mm_size / 2 + (mm->px_col + mm->off_x)
		/ mm->sq_size;
	mm->row_check = mm->pos_y - mm->mm_size / 2 + (mm->px_row + mm->off_y)
		/ mm->sq_size;
	if (mm->col_check < 0 || mm->row_check < 0 || mm->col_check >= p->mwidth
		|| mm->row_check >= p->mheight)
	{
		put_pixel(p, mm->px_row, mm->px_col, 0x111111);
		return (true);
	}
	return (false);
}

void	paint_grid(t_params *p, t_minimap *mm)
{
	if (mm->px_row == 0 || mm->px_row == mm->total_size - 1
		|| mm->px_col == 0 | mm->px_col == mm->total_size - 1)
		put_pixel(p, mm->px_row, mm->px_col, 0x000000);
	else if ((mm->px_row + mm->off_y) % mm->sq_size == 0 || (mm->px_col
			+ mm->off_x) % mm->sq_size == 0)
		put_pixel(p, mm->px_row, mm->px_col, 0xffffff);
}

void	fill_grid(t_params *p, t_minimap *mm)
{
	mm->px_col = -1;
	while (++mm->px_col < mm->total_size)
	{
		mm->px_row = -1;
		while (++mm->px_row < mm->total_size)
		{
			paint_grid(p, mm);
			if (unbounded(p, mm))
				continue ;
			paint_block(p, mm);
			paint_player(p, mm);
		}
	}
}

void	init_minimap(t_params *p, t_minimap *mm)
{
	mm->sq_size = 15;
	mm->mm_size = 8;
	mm->total_size = mm->sq_size * mm->mm_size;
	mm->heading = p->player.heading;
	mm->pos_y = p->player.position[0];
	mm->pos_x = p->player.position[1];
	mm->off_y = (p->player.position[0] - mm->pos_y) * mm->sq_size;
	mm->off_x = (p->player.position[1] - mm->pos_x) * mm->sq_size;
	mm->half_projection_plane_width = tan(p->fov / 2.0);
	mm->heading_x = sin(mm->heading);
	mm->heading_y = -cos(mm->heading);
	mm->plane_x = -mm->heading_y * mm->half_projection_plane_width;
	mm->plane_y = mm->heading_x * mm->half_projection_plane_width;
}

void	paint_mm(t_params *p, t_minimap *mm)
{
	mm->px_col = -1;
	while (++mm->px_col < mm->total_size)
	{
		mm->px_row = -1;
		while (++mm->px_row < mm->total_size)
			put_pixel(p, mm->px_row, mm->px_col, 0xa9a9a9);
	}
}

void	draw_minimap(t_params *p)
{
	t_minimap	mm;

	init_minimap(p, &mm);
	paint_mm(p, &mm);
	draw_rays(p, &mm);
	fill_grid(p, &mm);
}

void	look_up_down(t_params *params, int direction)
{
	if (direction > 0 && params->player.vert_angle < M_PI / 6)
		params->player.vert_angle += M_PI / 180;
	else if (direction < 0 && params->player.vert_angle > -M_PI / 6)
		params->player.vert_angle -= M_PI / 180;
}

void	move(t_params *params)
{
	move_player(params, params->player.move_ws);
	strafe_player(params, params->player.move_ad);
	rotate_player(params, params->player.move_turn);
	look_up_down(params, params->player.move_tilt);
}

int	render(t_params *p)
{
	move(p);
	draw_walls(p);
	draw_crosshair(p);
	draw_minimap(p);
	mlx_put_image_to_window(p->mlx.ptr, p->mlx.win, p->mlx.img, 0, 0);
	return (1);
}
