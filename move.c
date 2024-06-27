/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:28:05 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 14:28:05 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	explorable(t_params *params, double y, double x);

void	rotate_player(t_params *params, float degrees)
{
	t_player	*player;

	player = &(params->player);
	player->heading += M_PI / 180 * degrees;
}

void	look_up_down(t_params *params, int direction)
{
	if (direction > 0 && params->player.vert_angle < M_PI / 6)
		params->player.vert_angle += M_PI / 180;
	else if (direction < 0 && params->player.vert_angle > -M_PI / 6)
		params->player.vert_angle -= M_PI / 180;
}

void	move_player(t_params *params, float direction)
{
	t_player	*player;
	t_move		move;

	player = &(params->player);
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

	player = &(params->player);
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

void	move(t_params *params)
{
	move_player(params, params->player.move_ws);
	strafe_player(params, params->player.move_ad);
	rotate_player(params, params->player.move_turn);
	look_up_down(params, params->player.move_tilt);
}
