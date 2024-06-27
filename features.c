/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   features.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:31:41 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 15:31:43 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	spraypaint(t_params *params)
{
	t_player	*player;
	double		heading;
	int			map_x;
	int			map_y;

	player = &(params->player);
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

void	toggle_door(t_params *params)
{
	t_player	*player;
	double		heading;
	int			map_x;
	int			map_y;

	player = &(params->player);
	heading = player->heading;
	map_x = player->position[1] + sin(heading);
	map_y = player->position[0] - cos(heading);
	if (params->map[map_y][map_x] == 'D')
		params->map[map_y][map_x] = 'd';
	else if (params->map[map_y][map_x] == 'd')
		params->map[map_y][map_x] = 'D';
}

void	build_wall(t_params *params)
{
	t_player	*player;
	double		heading;
	int			map_x;
	int			map_y;
	char		grid;

	player = &(params->player);
	heading = player->heading;
	map_x = player->position[1] + sin(heading);
	map_y = player->position[0] - cos(heading);
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

void	translate_player(t_player *player, float delta)
{
	if (delta > 0 && (player->god || player->height < 0.8))
		player->height += delta;
	else if (delta < 0 && (player->god || player->height > 0.2))
		player->height += delta;
}
