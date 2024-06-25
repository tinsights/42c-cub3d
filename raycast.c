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


void reset_ray(t_ray *ray, t_player* player)
{
	ray->id = 1;
	ray->projection_plane_x = 2.0 * ((double) ray->col) / (WIN_WIDTH - 1) - 1; // goes from -1 to 1.

	ray->dir_x = ray->heading_x + ray->projection_plane_x * ray->plane_x;
	ray->dir_y = ray->heading_y + ray->projection_plane_x * ray->plane_y;

	ray->delta_dist_x = (ray->dir_x == 0) ? INFINITY : fabs(1 / ray->dir_x);
	ray->delta_dist_y = (ray->dir_y == 0) ? INFINITY : fabs(1 / ray->dir_y);

	ray->map_x = (int) player->position[1];
	ray->map_y = (int) player->position[0];
	ray->hit = false;

	ray->side_x = false;
	ray->perp_wall_dist = 0.0;
	ray->step_x = 0;
	ray->step_y = 0;
	ray->dist_x = 0.0;
	ray->dist_y = 0.0;
	ray->next = NULL;

}

void direct_ray(t_ray *ray, t_player *player)
{
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
}

bool is_wall(char c)
{
	return (c == '1' || c == '2' || c == 'D' || c == 'a');
}

void inner_wall(t_params *params, t_ray* ray)
{
    if (is_wall(params->map[ray->map_y][ray->map_x]))
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
}

void cast_to_wall(t_params *params, t_ray *ray)
{
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
}

void ray_pierce(t_params *params, t_ray *ray)
{
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
        dda(params, next);
    }
}

void dda(t_params *params, t_ray *ray)
{
	inner_wall(params, ray);
    cast_to_wall(params, ray);
	if (!params->lights && (params->player->height > 1.0 || params->player->height < 0))
	    ray_pierce(params, ray);
}

int brightness_adj(int col, float brightness)
{
	int r;
	int g;
	int b;
	int color;

	r = (col >> 16) & 0xFF;
	g = (col >> 8) & 0xFF;
	b = col & 0xFF;
	r *= brightness;
	g *= brightness;
	b *= brightness;
	color = ((r << 16) | (g << 8) | b);
	return color;
}

void get_wall_dimensions(t_params *params, t_player *player, t_ray *ray, t_wall *wall)
{
	wall->pos_x = player->position[1];
	wall->pos_y = player->position[0];
	
	wall->dist_to_pp = (WIN_WIDTH / 2.0) / (tan(params->fov / 2.0));
	wall->ratio = wall->dist_to_pp / ray->perp_wall_dist;
	wall->vert_shear = tan(player->vert_angle) * wall->dist_to_pp;

	wall->actual_bottom = wall->ratio * player->height + WIN_HEIGHT / 2 + wall->vert_shear;
	wall->actual_top = wall->actual_bottom - wall->ratio;

	wall->bottom_of_wall = wall->actual_bottom;
	if (wall->bottom_of_wall >= WIN_HEIGHT)
		wall->bottom_of_wall = WIN_HEIGHT - 1;
	wall->top_of_wall = wall->actual_top;
	if (wall->top_of_wall < 0)
		wall->top_of_wall = 0;
}

void get_texture(t_ray *ray, t_wall *wall)
{
	if (ray->side_x)
	{
		wall->texture_slice = wall->pos_y + ray->dir_y * ray->perp_wall_dist - ray->map_y;
		if (ray->dir_x < 0)
			wall->texture_slice = 1.0 - wall->texture_slice;
	}
	else
	{
		wall->texture_slice = wall->pos_x + ray->dir_x * ray->perp_wall_dist - ray->map_x;
		if (ray->dir_y > 0)
			wall->texture_slice = 1.0 - wall->texture_slice;
	}
	
	ray->img_data = (unsigned int *)ray->img->data;

	wall->tex_col = wall->texture_slice * (double) ray->img->width;

	wall->true_line_height = wall->actual_bottom - wall->actual_top;

    wall->dist = ray->perp_wall_dist;
	if (wall->dist < 1.0)
		wall->dist = 1.0;
    wall->brightness = 1 / wall->dist;
}

void paint_col(t_params *params, t_ray *ray, t_wall *wall, int px)
{

	int ceiling_color = params->cclr;

	if (params->lights)
		ceiling_color = brightness_adj(ceiling_color, wall->brightness);
	if (!(ray->next))
	{
		if (px < wall->top_of_wall )
			put_pixel(*params, px, ray->col, ceiling_color);
		else if (px > wall->bottom_of_wall ) // floor
			put_pixel(*params, px, ray->col, params->fclr);
	}
	if (px >= wall->top_of_wall && px <= wall->bottom_of_wall)
	{
		wall->row_slice = (double) (px - wall->actual_top) / wall->true_line_height;
		wall->tex_row = wall->row_slice * (double) ray->img->height;
		wall->color = ray->img_data[(wall->tex_row * ray->img->size_line)/(ray->img->bpp /8) + wall->tex_col];
		if (params->lights)
			wall->color = brightness_adj(wall->color, wall->brightness);
		put_pixel(*params, px, ray->col, wall->color);
	}
}

void paint_walls(t_params *params, t_player *player, t_ray *ray)
{
	t_wall wall;

	if (ray->next)
	{
		paint_walls(params, player, ray->next);
		free(ray->next);
	}
	get_wall_dimensions(params, player, ray, &wall);
	get_texture(ray, &wall);
	for (int px = 0; px < WIN_HEIGHT; px++)
	{
		paint_col(params, ray, &wall, px);
	}
}


void draw_walls(t_params *params)
{
	t_player *player = params->player;
	t_ray ray;

	ray.heading_x = sin(player->heading);
	ray.heading_y = -cos(player->heading);
	params->half_plane_width = tan(params->fov / 2.0); // todo: store in params, change during zoom

	ray.plane_x = -ray.heading_y * params->half_plane_width;
	ray.plane_y = ray.heading_x * params->half_plane_width;

	for (int col = 0; col < WIN_WIDTH; col++)
	{
		ray.col = col;
		reset_ray(&ray, player);
		direct_ray(&ray, player);
		dda(params, &ray);
		paint_walls(params, player, &ray);
	}
}
