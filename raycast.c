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
	double half_projection_plane_width = tan(params->fov / 2.0);
	// printf("ppw: %f\n", half_projection_plane_width);

	double right_x = -forward_y * half_projection_plane_width;
	double right_y = forward_x * half_projection_plane_width;

    double pos_x = player->position[1];
    double pos_y = player->position[0];
	
	for (int col = 0; col < WIN_WIDTH; col++)
	{
		double relative_distance_along_projection_plane = 2.0 * ((double) col) / (WIN_WIDTH - 1) - 1; // goes from -1 to 1.

		double ray_dir_x = forward_x + relative_distance_along_projection_plane * right_x;
		double ray_dir_y = forward_y + relative_distance_along_projection_plane * right_y;

		double delta_dist_x = (ray_dir_x == 0) ? INFINITY : fabs(1 / ray_dir_x);
		double delta_dist_y = (ray_dir_y == 0) ? INFINITY : fabs(1 / ray_dir_y);

        int map_x = (int) pos_x;
        int map_y = (int) pos_y;

        bool hit = false;

		int side_x = 0;
        double perp_wall_distance = 0.0;
		
        /**
         * Set initial parameters based on heading
        */
        int step_x = 0;
		int step_y = 0;
		double dist_x = 0.0;
		double dist_y = 0.0;
		t_img *image;

		if (ray_dir_x < 0) // looking "west"
		{
			step_x = -1;
			dist_x = (pos_x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			dist_x = (1.0 + map_x - pos_x) * delta_dist_x;
		}
		if (ray_dir_y < 0) // looking "north"
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
				side_x = 1;
			}
			else
			{
				dist_y += delta_dist_y;
				side_x = 0;
			}
			hit = true;
			image = params->inner;
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
				side_x = 1;
			}
			else
			{
				dist_y += delta_dist_y;
				map_y += step_y;
				side_x = 0;
			}
			if (map[map_y][map_x] != 0)
			{
				hit = true;
				if (side_x)
				{
					if (ray_dir_x > 0)
						image = params->east;
					else
						image = params->west;
				}
				else
				{
					if (ray_dir_y > 0)
						image = params->south;
					else
						image = params->north;
				}
			}
		}
		unsigned int *img_data = (unsigned int *)image->data;

		if (side_x)
			perp_wall_distance = dist_x - delta_dist_x;
		else
			perp_wall_distance = dist_y - delta_dist_y;

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

		double texture_slice;
		if (side_x)
		{
			texture_slice = pos_y + ray_dir_y * perp_wall_distance - map_y;
			if (ray_dir_x < 0)
				texture_slice = 1.0 - texture_slice;
		}
		else
		{
			texture_slice = pos_x + ray_dir_x * perp_wall_distance - map_x;
			if (ray_dir_y > 0)
				texture_slice = 1.0 - texture_slice;
		}
		
        /* -------------------------------------------------------------------------- */
        /*                             Painting the walls                             */
        /* -------------------------------------------------------------------------- */

		int color = map[map_y][map_x] == 1 ? 0x000088 : 0x880000;
		
		int tex_col = texture_slice * (double) image->width;

		// if (col == WIN_WIDTH / 2)
		// {
		// 	printf("looking at wall %i %i, at slice %f\n", map_x, map_y, texture_slice);
		// 	printf("ray dir x: %f | ray diry: %f\n", ray_dir_x, ray_dir_y);
		// 	printf("%d %x\n", ((unsigned int*)image->data)[0], ((unsigned int*)image->data)[0]);
		// 	printf("image bpp: %i, image width: %i, image height: %i, image line_sz: %i\n", image->bpp, image->width, image->height, image->size_line);
		// 	printf("tex col is %i\n", tex_col);
		// }


		double true_line_height = actual_bottom - actual_top;
        if (side_x)
			color /= 2;
		for (int px = 0; px < WIN_HEIGHT; px++)
		{
			if (px < top_of_wall )
				put_pixel(*params, px, col, 0x888888);
			else if (px> bottom_of_wall )
				put_pixel(*params, px, col, 0x333333);
			else
			{
				double row_slice = (double) (px - actual_top) / true_line_height;
				// printf("row slice %f\n", row_slice);
				int tex_row = row_slice * (double) image->height;
				put_pixel(*params, px, col, (img_data[(tex_row * image->size_line)/(image->bpp /8) + tex_col]));
			}
		}
	}
}