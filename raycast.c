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


void initialise_ray(t_ray *ray, t_player *player, int col)
{
	ray->id = 1;
	ray->col = col;
	ray->projection_plane_x = 2.0 * ((double) col) / (WIN_WIDTH - 1) - 1; // goes from -1 to 1.

	ray->dir_x = ray->heading_x + ray->projection_plane_x * ray->plane_x;
	ray->dir_y = ray->heading_y + ray->projection_plane_x * ray->plane_y;

	ray->delta_dist_x = (ray->dir_x == 0) ? INFINITY : fabs(1 / ray->dir_x);
	ray->delta_dist_y = (ray->dir_y == 0) ? INFINITY : fabs(1 / ray->dir_y);

	ray->map_x = (int) player->position[1];
	ray->map_y = (int) player->position[0];

	ray->hit = false;

	ray->side_x = false;
	ray->perp_wall_dist = 0.0;
	
	/**
	 * Set initial parameters based on heading
	 * can do bzero
	*/
	ray->step_x = 0;
	ray->step_y = 0;
	ray->dist_x = 0.0;
	ray->dist_y = 0.0;

	if (ray->dir_x < 0) // looking "west"
	{
		ray->step_x = -1;
		ray->dist_x = (player->position[1] - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->dist_x = (1.0 + ray->map_x - player->position[1]) * ray->delta_dist_x;
	}
	if (ray->dir_y < 0) // looking "north"
	{
		ray->step_y = -1;
		ray->dist_y = (player->position[0] - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->dist_y = (1.0 + ray->map_y - player->position[0]) * ray->delta_dist_y;
	}
	ray->next = NULL;
}

bool is_wall(char c)
{
	return (c == '1' || c == '2' || c == 'D');
}

void dda(t_params *params, t_ray *ray)
{

		// if (ray->id > 1 && ray->col)
		// 	printf("casting %i ray\n", ray->id);
        /**
         * This just checks if we are currently inside a box
         * and renders the walls from within.
         * not exactly neccessary, especially once wall collision is implemented
        */
		if (is_wall(p->map[ray->map_y][ray->map_x]))
		{
			if (ray->dist_x < ray->dist_y) // what if equal?
			{
				ray->dist_x += ray->delta_dist_x;
				ray->side_x = 1;
			}
			else
			{
				ray->dist_y += ray->delta_dist_y;
				ray->side_x = 0;
			}
			ray->hit = true;
			ray->img = params->inner;
		}

        /* -------------------------------------------------------------------------- */
        /*                        DIGITAL DIFFERENTIAL ANALYSIS                       */
        /* -------------------------------------------------------------------------- */
		while(!ray->hit)
		{
			if (ray->dist_x < ray->dist_y) // what if equal?
			{
				ray->dist_x += ray->delta_dist_x;
				ray->map_x += ray->step_x;
				ray->side_x = 1;
			}
			else
			{
				ray->dist_y += ray->delta_dist_y;
				ray->map_y += ray->step_y;
				ray->side_x = 0;
			}
			if (params->map[ray->map_y][ray->map_x] != '0')
			{
				ray->hit = true;
				if (ray->side_x)
				{
					if (ray->dir_x > 0)
						ray->img = params->east;
					else
						ray->img = params->west;
				}
				else
				{
					if (ray->dir_y > 0)
						ray->img = params->south;
					else
						ray->img = params->north;
				}
			}
			if (params->map[ray->map_y][ray->map_x] == 't' || params->map[ray->map_y][ray->map_x] == 'T')
				ray->img = params->spray;
			else if (params->map[ray->map_y][ray->map_x] == 'D')
				ray->img = params->door;
			if (params->map[ray->map_y][ray->map_x] == 'd')
				ray->hit = false;

		}

		if (ray->side_x)
			ray->perp_wall_dist = ray->dist_x - ray->delta_dist_x;
		else
			ray->perp_wall_dist = ray->dist_y - ray->delta_dist_y;

		if (params->player->height > 1.0 || params->player->height < 0)
		{
			// if ((ray->map_x > 0 && ray->map_x < MWIDTH - 1)
			// 	&& (ray->map_y > 0 && ray->map_y < MHEIGHT - 1))
			if ((ray->map_x > 0 && ray->map_x < params->mwidth - 1)
				&& (ray->map_y > 0 && ray->map_y < params->mheight - 1))
			{
				t_ray *next = ft_calloc(1, sizeof(t_ray));
				ray->next = next;

				next->id = ray->id + 1;
				next->col = ray->col;
				next->dir_x = ray->dir_x;
				next->dir_y = ray->dir_y;
				next->step_x = ray->step_x;
				next->step_y = ray->step_y;
				next->map_x = ray->map_x;
				next->map_y = ray->map_y;
				next->dist_x = ray->dist_x;
				next->dist_y = ray->dist_y;
				next->delta_dist_x = ray->delta_dist_x;
				next->delta_dist_y = ray->delta_dist_y;
				next->hit = 0;
				if (ray->img == params->inner)
				{
					if (ray->side_x)
						next->map_x += ray->step_x;
					else
						next->map_y += ray->step_y;
				}
				// if (ray->col == WIN_WIDTH / 2)
				// 	printf("new ray starting from %i %i\n", next->map_x, next->map_y);
				dda(params, next);
			}
			else if(ray->col == WIN_WIDTH / 2)
				printf("\tplayer %f %f, ray %i %i\n", params->player->position[1],params->player->position[0], ray->map_x, ray->map_y);
		}
}


void paint_walls(t_params *params, t_player *player, t_ray *ray, int col)
{
		if (ray->next)
		{
			paint_walls(params, player, ray->next, col);
			free(ray->next);
		}
		// if (ray->col == WIN_WIDTH / 2)
		// 	printf("painting ray %i\n", ray->id);
		double pos_x = player->position[1];
		double pos_y = player->position[0];
		
		double dist_to_projection_plane = (WIN_WIDTH / 2.0) / (tan(params->fov / 2.0));
		double ratio = dist_to_projection_plane / ray->perp_wall_dist;
		double vert_shear = tan(player->vert_angle) * dist_to_projection_plane;

		int actual_bottom = ratio * player->height + WIN_HEIGHT / 2 + vert_shear;
		int actual_top = actual_bottom - ratio;

		int bottom_of_wall = actual_bottom;
		if (bottom_of_wall >= WIN_HEIGHT)
			bottom_of_wall = WIN_HEIGHT - 1;
		int top_of_wall = actual_top;
		if (top_of_wall < 0)
			top_of_wall = 0;

		// printf("unit height is %i, ray->perp_wall_dist is %f\n", unit_height, ray->perp_wall_dist);
		// printf("lineheight is %i, bottom of wall is %i, top of wall is %i\n", lineHeight, bottom_of_wall, top_of_wall);

		double texture_slice;
		if (ray->side_x)
		{
			texture_slice = pos_y + ray->dir_y * ray->perp_wall_dist - ray->map_y;
			if (ray->dir_x < 0)
				texture_slice = 1.0 - texture_slice;
		}
		else
		{
			texture_slice = pos_x + ray->dir_x * ray->perp_wall_dist - ray->map_x;
			if (ray->dir_y > 0)
				texture_slice = 1.0 - texture_slice;
		}
		
		/* -------------------------------------------------------------------------- */
		/*                             PAINTING THE WALLS                             */
		/* -------------------------------------------------------------------------- */

		// int color = map[ray->map_y][ray->map_x] == 1 ? 0x000088 : 0x880000;
		

		// if (col == WIN_WIDTH / 2)
		// {
		// 	printf("looking at wall %i %i, at slice %f\n", ray->map_x, ray->map_y, texture_slice);
		// 	printf("ray dir x: %f | ray diry: %f\n", ray_dir_x, ray_dir_y);
		// 	printf("%d %x\n", ((unsigned int*)image->data)[0], ((unsigned int*)image->data)[0]);
		// 	printf("image bpp: %i, image width: %i, image height: %i, image line_sz: %i\n", image->bpp, image->width, image->height, image->size_line);
		// 	printf("tex col is %i\n", tex_col);
		// }
		ray->img_data = (unsigned int *)ray->img->data;

		int tex_col = texture_slice * (double) ray->img->width;

		double true_line_height = actual_bottom - actual_top;
		// if (ray->col == WIN_WIDTH / 2)
		// 	printf("ray %i has tlh of %f\n", ray->id, true_line_height);

		// bool condition = ray->img == params->inner;
		for (int px = 0; px < WIN_HEIGHT; px++)
		{

			// if (condition)
			// {
			// 	put_pixel(*params, px, col, 0xfffff);
			// }
			if (!(ray->next))
			{
				if (px < top_of_wall ) // ceiling
					//put_pixel(*params, px, col, 0x888888);
					put_pixel(*params, px, col, params->cclr);
				else if (px > bottom_of_wall ) // floor
					//put_pixel(*params, px, col, 0x333333);
					put_pixel(*params, px, col, params->fclr);
			}
			if (px >= top_of_wall && px <= bottom_of_wall)
			{
				double row_slice = (double) (px - actual_top) / true_line_height;
				int tex_row = row_slice * (double) ray->img->height;
				put_pixel(*params, px, col, (ray->img_data[(tex_row * ray->img->size_line)/(ray->img->bpp /8) + tex_col]));
			}
		}
		// }
}

void draw_walls(t_params *params)
{
	t_player *player = params->player;
	t_ray ray;

	// ft_bzero(&ray, sizeof(t_ray));

	ray.heading_x = sin(player->heading);
	ray.heading_y = -cos(player->heading);
	double half_projection_plane_width = tan(params->fov / 2.0);
	// printf("ppw: %f\n", half_projection_plane_width);

	ray.plane_x = -ray.heading_y * half_projection_plane_width;
	ray.plane_y = ray.heading_x * half_projection_plane_width;

   
	
	for (int col = 0; col < WIN_WIDTH; col++)
	{
		initialise_ray(&ray, player, col);

		dda(params, &ray);

		// if (ray.next)
		// {
		// 	if (col == WIN_WIDTH / 2)
		// 	{	
		// 		t_ray *ptr = &ray;

		// 		while (ptr)
		// 		{
		// 			printf("ray %i hit %i %i with dist %f\n", ptr->id, ptr->map_x, ptr->map_y, ptr->perp_wall_dist);
		// 			ptr = ptr->next;
		// 		}
		// 	}
		// }

		paint_walls(params, player, &ray, col);
	}
}
