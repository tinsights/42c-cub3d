/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:20:08 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 15:20:09 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	translate_player(t_player *player, float delta);
void	build_wall(t_params *params);
void	toggle_door(t_params *params);
void	spraypaint(t_params *params);

void	move_keys(int keycode, t_params *params)
{
	if (keycode == XK_w || keycode == XK_W)
		params->player.move_ws = -1;
	else if (keycode == XK_a || keycode == XK_A)
		params->player.move_ad = -1;
	else if (keycode == XK_s || keycode == XK_S)
		params->player.move_ws = 1;
	else if (keycode == XK_d || keycode == XK_D)
		params->player.move_ad = 1;
	else if (keycode == XK_Left)
		params->player.move_turn = -1;
	else if (keycode == XK_Right)
		params->player.move_turn = 1;
	else if (keycode == XK_KP_Add)
		params->fov += M_PI / 30;
	else if (keycode == XK_KP_Subtract)
		params->fov -= M_PI / 30;
	else if (keycode == XK_Up)
		params->player.move_tilt = 1;
	else if (keycode == XK_Down)
		params->player.move_tilt = -1;
	else if (keycode == XK_Control_L)
		translate_player(&(params->player), -0.25);
	else if (keycode == XK_space)
		translate_player(&(params->player), 0.25);
}

int	key_hook(int keycode, t_params *params)
{
	t_player	*player;

	player = &(params->player);
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
		toggle_door(params);
	else if (keycode == XK_g)
		player->god = !player->god;
	else if (keycode == XK_f)
		params->lights = !params->lights;
	return (1);
}

int	key_release_hook(int keycode, t_params *params)
{
	if (keycode == XK_w)
		params->player.move_ws = 0;
	else if (keycode == XK_a)
		params->player.move_ad = 0;
	else if (keycode == XK_s)
		params->player.move_ws = 0;
	else if (keycode == XK_d)
		params->player.move_ad = 0;
	else if (keycode == XK_Left)
		params->player.move_turn = 0;
	else if (keycode == XK_Right)
		params->player.move_turn = 0;
	else if (keycode == XK_Up)
		params->player.move_tilt = 0;
	else if (keycode == XK_Down)
		params->player.move_tilt = 0;
	return (1);
}

int	mouse_click(int button, int x, int y, t_params *params)
{
	if (button == 2)
	{
		params->clicked_px[0] = x;
		params->clicked_px[1] = y;
	}
	else if (button == 4)
		params->fov -= M_PI / 30;
	else if (button == 5)
		params->fov += M_PI / 30;
	else if (button == 3)
		build_wall(params);
	return (1);
}

int	mouse_move(int x, int y, t_params *params)
{
	t_player	*player;
	int			x_delta;
	int			y_delta;
	double		heading_delta;
	double		vert_delta;

	player = &(params->player);
	x_delta = x - params->clicked_px[0];
	y_delta = y - params->clicked_px[1];
	heading_delta = (double)x_delta / WIN_WIDTH * tan(params->fov / 2.0);
	vert_delta = (double)y_delta / WIN_WIDTH * tan(params->fov / 2.0);
	params->player.heading += 4.0 * heading_delta;
	player->vert_angle -= 4.0 * vert_delta;
	params->clicked_px[0] = x;
	params->clicked_px[1] = y;
	return (1);
}
