/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 10:57:44 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/10 10:57:47 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void draw_walls(t_params *params)
{
	t_player *player = params->player;

	double forward_x = sin(player->heading);
	double forward_y = -cos(player->heading);

	// half projection plane width helps to get rid of warping
	double half_projection_plane_width = tan(params->fov / 2.0);
	// printf("ppw: %f\n", half_projection_plane_width);

	double right_x = -forward_y * half_projection_plane_width;
	double right_y = forward_x * half_projection_plane_width;

    double pos_x = player->position[1];
    double pos_y = player->position[0];

	for (int col = 0; col < WIN_WIDTH; col++)
	{
		double relative_distance_along_projection_plane = 2.0 * ((double) col) / (WIN_WIDTH - 1) - 1; // goes from -1 to 1.

		double rayDir_x = forward_x + relative_distance_along_projection_plane * right_x;
		double rayDir_y = forward_y + relative_distance_along_projection_plane * right_y;


		// using some math shortcuts
		// mostly from lodev
		double delta_dist_x = (rayDir_x == 0) ? INFINITY : fabs(1 / rayDir_x);
		double delta_dist_y = (rayDir_y == 0) ? INFINITY : fabs(1 / rayDir_y);

        int map_x = (int) pos_x;
        int map_y = (int) pos_y;

        bool hit = false;

		int side = 0;
        double perp_wall_distance = 0.0;
		
        /**
         * Set initial parameters based on heading
        */
        int step_x = 0;
		int step_y = 0;
		double dist_x = 0.0;
		double dist_y = 0.0;

		if (rayDir_x < 0) // looking "west"
		{
			step_x = -1;
			dist_x = (pos_x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			dist_x = (1.0 + map_x - pos_x) * delta_dist_x;
		}
		if (rayDir_y < 0) // looking "north"
		{
			step_y = -1;
			dist_y = (pos_y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			dist_y = (1.0 + map_y - pos_y) * delta_dist_y;
		}


        /**
         * This just checks if we are currently inside a box
         * and renders the walls from within.
         * not exactly neccessary, especially once wall collision is implemented
        */
		if (map[map_y][map_x] != 0)
		{
			if (dist_x < dist_y) // what if equal?
			{
				dist_x += delta_dist_x;
				side = 1;
			}
			else
			{
				dist_y += delta_dist_y;
				side = 0;
			}
			hit = true;
		}

        /* -------------------------------------------------------------------------- */
        /*                        Digital Differential Analysis                       */
        /* -------------------------------------------------------------------------- */
		while(!hit)
		{
			if (dist_x < dist_y) // what if equal?
			{
				dist_x += delta_dist_x;
				map_x += step_x;
				side = 1;
			}
			else
			{
				dist_y += delta_dist_y;
				map_y += step_y;
				side = 0;
			}
			if (map[map_y][map_x] != 0)
			{
				hit = true;
			}
		}

		if (side)
			perp_wall_distance = dist_x - delta_dist_x;
		else
			perp_wall_distance = dist_y - delta_dist_y;


		if (col == WIN_WIDTH / 2)
		{
			//printf("looking at %f\n", )
		}
		double dist_to_projection_plane = (WIN_WIDTH / 2.0) / (tan(params->fov / 2.0));
		double ratio = dist_to_projection_plane / perp_wall_distance;
		double vert_shear = tan(player->vert_angle) * dist_to_projection_plane;

		int actual_bottom = ratio * player->height + WIN_HEIGHT / 2 + vert_shear;
		int actual_top = actual_bottom - ratio;

		int bottom_of_wall = actual_bottom;
		if (bottom_of_wall >= WIN_HEIGHT)
			bottom_of_wall = WIN_HEIGHT - 1;
		int top_of_wall = actual_top;
		if (top_of_wall < 0)
			top_of_wall = 0;

		// printf("unit height is %i, perp_wall_distance is %f\n", unit_height, perp_wall_distance);
		// printf("lineheight is %i, bottom of wall is %i, top of wall is %i\n", lineHeight, bottom_of_wall, top_of_wall);

        /* -------------------------------------------------------------------------- */
        /*                             Painting the walls                             */
        /* -------------------------------------------------------------------------- */
		int color = map[map_y][map_x] == 1 ? 0x000088 : 0x880000;
        if (side)
			color /= 2;
		for (int px = 0; px < WIN_HEIGHT; px++)
		{
			if (px < top_of_wall )
				put_pixel(*params, px, col, 0x888888);
			else if (px> bottom_of_wall )
				put_pixel(*params, px, col, 0x333333);
			else
				put_pixel(*params, px, col, color);
		}
	}
}
