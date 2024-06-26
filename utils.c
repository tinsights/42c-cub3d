/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:09:43 by tjegades          #+#    #+#             */
/*   Updated: 2024/05/21 13:09:45 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define STEP 0.02

void	put_pixel(t_params *p, t_uint row, t_uint col, int colour)
{
	char	*px;

	px = p->mlx->img_addr + row * p->mlx->line_sz + col * (p->mlx->bpp / 8);
	*(t_uint *)px = mlx_get_color_value(p->mlx->ptr, colour);
}

int	close_window(t_params *params)
{
	mlx_do_key_autorepeaton(params->mlx->ptr);
	free_strarr(params->input->map);
	free(params->input->nxpm);
	free(params->input->sxpm);
	free(params->input->expm);
	free(params->input->wxpm);
	free(params->input);
	mlx_destroy_image(params->mlx->ptr, params->mlx->img);
	mlx_destroy_image(params->mlx->ptr, params->north);
	mlx_destroy_image(params->mlx->ptr, params->east);
	mlx_destroy_image(params->mlx->ptr, params->west);
	mlx_destroy_image(params->mlx->ptr, params->south);
	mlx_destroy_image(params->mlx->ptr, params->spray);
	mlx_destroy_image(params->mlx->ptr, params->door);
	mlx_destroy_image(params->mlx->ptr, params->inner);
	mlx_destroy_window(params->mlx->ptr, params->mlx->win);
	mlx_destroy_display(params->mlx->ptr);
	free(params->mlx->ptr);
	exit(1);
	return (1);
}

void	rotate_player(t_params *params, float degrees)
{
	t_player	*player;

	player = params->player;
	player->heading += M_PI / 180 * degrees;
}

bool	explorable(t_params *params, double y, double x)
{
	if (x < 0 || x > params->mwidth || y < 0 || y > params->mheight)
		return (false);
	return (params->player->god || !is_wall(params->map[(int)y][(int)x]));
}

typedef struct s_move
{
	float	horiz_step;
	float	vert_step;
	float	new_height;
	float	curr_y;
	float	curr_x;
	float	heading;
	float	new_y;
	float	new_x;
}			t_move;

void	move_player(t_params *params, float direction)
{
	t_player	*player;
	t_move		move;

	player = params->player;
	move.horiz_step = STEP * cos(player->vert_angle);
	move.vert_step = STEP * -sin(player->vert_angle);
	move.new_height = player->height + move.vert_step * direction;
	if (player->god)
		player->height = move.new_height;
	move.curr_y = player->position[0];
	move.curr_x = player->position[1];
	move.heading = player->heading;
	move.new_y = move.curr_y + cos(move.heading) * move.horiz_step * direction;
	move.new_x = move.curr_x + -sin(move.heading) * move.horiz_step * direction;
	if (explorable(params, move.curr_y, move.new_x))
	{
		player->position[1] = move.new_x;
		move.curr_x = move.new_x;
	}
	if (explorable(params, move.new_y, move.curr_x))
	{
		if (move.new_y > 0 && move.new_y < params->mheight)
			player->position[0] = move.new_y;
	}
}

void	strafe_player(t_params *params, float direction)
{
	t_player	*player;
	t_move		move;

	player = params->player;
	move.curr_y = player->position[0];
	move.curr_x = player->position[1];
	move.heading = player->heading;
	move.new_y = move.curr_y + sin(move.heading) * STEP * direction;
	move.new_x = move.curr_x + cos(move.heading) * STEP * direction;
	if (explorable(params, move.curr_y, move.new_x))
	{
		player->position[1] = move.new_x;
		move.curr_x = move.new_x;
	}
	if (explorable(params, move.new_y, move.curr_x))
	{
		if (move.new_y > 0 && move.new_y < params->mheight)
			player->position[0] = move.new_y;
	}
}

void	spraypaint(t_params *params)
{
	t_player	*player;
	double		heading;
	int			map_x;
	int			map_y;

	player = params->player;
	heading = player->heading;
	map_x = player->position[1] + sin(heading);
	map_y = player->position[0] - cos(heading);
	if (params->map[map_y][map_x] == '1')
		params->map[map_y][map_x] = 't';
	else if (params->map[map_y][map_x] == 't')
		params->map[map_y][map_x] = '1';
	else if (params->map[map_y][map_x] == '2')
		params->map[map_y][map_x] = 'T';
	else if (params->map[map_y][map_x] == 'T')
		params->map[map_y][map_x] = '2';
}

void	door(t_params *params)
{
	t_player	*player;
	double		heading;
	int			map_x;
	int			map_y;

	player = params->player;
	heading = player->heading;
	map_x = player->position[1] + sin(heading);
	map_y = player->position[0] - cos(heading);
	if (params->map[map_y][map_x] == 'D')
		params->map[map_y][map_x] = 'd';
	else if (params->map[map_y][map_x] == 'd')
		params->map[map_y][map_x] = 'D';
}

void	translate_player(t_player *player, float delta)
{
	if (delta > 0 && (player->god || player->height < 0.8))
		player->height += delta;
	else if (delta < 0 && (player->god || player->height > 0.2))
		player->height += delta;
}

void	move_keys(int keycode, t_params *params)
{
	if (keycode == XK_w)
		params->player->move_ws = -1;
	else if (keycode == XK_a)
		params->player->move_ad = -1;
	else if (keycode == XK_s)
		params->player->move_ws = 1;
	else if (keycode == XK_d)
		params->player->move_ad = 1;
	else if (keycode == XK_Left)
		params->player->move_turn = -1;
	else if (keycode == XK_Right)
		params->player->move_turn = 1;
	else if (keycode == XK_KP_Add)
		params->fov += M_PI / 30;
	else if (keycode == XK_KP_Subtract)
		params->fov -= M_PI / 30;
	else if (keycode == XK_Up)
		params->player->move_tilt = 1;
	else if (keycode == XK_Down)
		params->player->move_tilt = -1;
	else if (keycode == XK_Control_L)
		translate_player(params->player, -0.25);
	else if (keycode == XK_space)
		translate_player(params->player, 0.25);
}

int	key_hook(int keycode, t_params *params)
{
	t_player	*player;

	player = params->player;
	if (keycode == XK_Escape)
		return (close_window(params));
	else if (keycode == XK_w || keycode == XK_W || keycode == XK_a
		|| keycode == XK_A || keycode == XK_s || keycode == XK_S
		|| keycode == XK_D || keycode == XK_d || keycode == XK_Left
		|| keycode == XK_Right || keycode == XK_Up || keycode == XK_Down
		|| keycode == XK_Control_L || keycode == XK_space)
		move_keys(keycode, params);
	else if (keycode == XK_t)
		spraypaint(params);
	else if (keycode == XK_e)
		door(params);
	else if (keycode == XK_g)
		player->god = !player->god;
	else if (keycode == XK_f)
		params->lights = !params->lights;
	return (1);
}

int	key_release_hook(int keycode, t_params *params)
{
	if (keycode == XK_w)
		params->player->move_ws = 0;
	else if (keycode == XK_a)
		params->player->move_ad = 0;
	else if (keycode == XK_s)
		params->player->move_ws = 0;
	else if (keycode == XK_d)
		params->player->move_ad = 0;
	else if (keycode == XK_Left)
		params->player->move_turn = 0;
	else if (keycode == XK_Right)
		params->player->move_turn = 0;
	else if (keycode == XK_Up)
		params->player->move_tilt = 0;
	else if (keycode == XK_Down)
		params->player->move_tilt = 0;
	return (1);
}

void	build_wall(t_params *params)
{
	t_player	*player;
	double		heading;
	int			map_x;
	int			map_y;
	char		grid;

	player = params->player;
	heading = player->heading;
	map_x = player->position[1] + sin(heading);
	map_y = player->position[0] - cos(heading);
	printf("%i %i %f %f\n", map_x, map_y, sin(heading), -cos(heading));
	grid = params->map[map_y][map_x];
	if (grid == '0')
	{
		map_x += sin(heading);
		map_y -= cos(heading);
	}
	if (params->map[map_y][map_x] == '0')
		params->map[map_y][map_x] = '2';
	else if (params->map[map_y][map_x] == '2')
		params->map[map_y][map_x] = '0';
}

int	mouse_click(int button, int x, int y, t_params *params)
{
	printf("button: %i | mouseX: %i | mouseY: %i\n", button, x, y);
	if (button == 2)
	{
		params->clicked_px[0] = x;
		params->clicked_px[1] = y;
	}
	else if (button == 4)
	{
		params->fov -= M_PI / 30;
		printf("zooming in\n");
	}
	else if (button == 5)
	{
		params->fov += M_PI / 30;
		printf("zooming out\n");
	}
	else if (button == 3)
	{
		build_wall(params);
	}
	return (1);
}

int	mouse_move(int x, int y, t_params *params)
{
	t_player	*player;
	int			x_delta;
	int			y_delta;
	double		heading_delta;
	double		vert_delta;

	player = params->player;
	x_delta = x - params->clicked_px[0];
	y_delta = y - params->clicked_px[1];
	heading_delta = (double)x_delta / WIN_WIDTH * tan(params->fov / 2.0);
	vert_delta = (double)y_delta / WIN_WIDTH * tan(params->fov / 2.0);
	params->player->heading += 4.0 * heading_delta;
	player->vert_angle -= 4.0 * vert_delta;
	params->clicked_px[0] = x;
	params->clicked_px[1] = y;
	return (1);
}

void	draw_crosshair(t_params *p)
{
	int	vert_center;
	int	horiz_center;
	int	half_crosshair_length;
	int	i;

	vert_center = WIN_HEIGHT / 2;
	horiz_center = WIN_WIDTH / 2;
	half_crosshair_length = 8;
	i = vert_center - half_crosshair_length;
	while (i < vert_center + half_crosshair_length)
		put_pixel(p, i++, horiz_center, 0x888800);
	i = horiz_center - half_crosshair_length;
	while (i < horiz_center + half_crosshair_length)
		put_pixel(p, vert_center, i++, 0x888800);
}
