/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:39:41 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 15:39:42 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycast.h"

int	brightness_adj(int col, float brightness)
{
	int	r;
	int	g;
	int	b;
	int	color;

	r = (col >> 16) & 0xFF;
	g = (col >> 8) & 0xFF;
	b = col & 0xFF;
	r *= brightness;
	g *= brightness;
	b *= brightness;
	color = ((r << 16) | (g << 8) | b);
	return (color);
}

void	get_wall_dimensions(t_params *params, t_player *player, t_ray *ray,
		t_wall *wall)
{
	wall->pos_x = player->position[1];
	wall->pos_y = player->position[0];
	wall->dist_to_pp = (WIN_WIDTH / 2.0) / (tan(params->fov / 2.0));
	wall->ratio = wall->dist_to_pp / ray->perp_wall_dist;
	wall->vert_shear = tan(player->vert_angle) * wall->dist_to_pp;
	wall->actual_bottom = wall->ratio * player->height + WIN_HEIGHT / 2
		+ wall->vert_shear;
	wall->actual_top = wall->actual_bottom - wall->ratio;
	wall->bottom_of_wall = wall->actual_bottom;
	if (wall->bottom_of_wall >= WIN_HEIGHT)
		wall->bottom_of_wall = WIN_HEIGHT - 1;
	wall->top_of_wall = wall->actual_top;
	if (wall->top_of_wall < 0)
		wall->top_of_wall = 0;
}

void	get_texture(t_ray *ray, t_wall *wall)
{
	if (ray->side_x)
	{
		wall->texture_slice = wall->pos_y + ray->dir_y * ray->perp_wall_dist
			- ray->map_y;
		if (ray->dir_x < 0)
			wall->texture_slice = 1.0 - wall->texture_slice;
	}
	else
	{
		wall->texture_slice = wall->pos_x + ray->dir_x * ray->perp_wall_dist
			- ray->map_x;
		if (ray->dir_y > 0)
			wall->texture_slice = 1.0 - wall->texture_slice;
	}
	ray->img_data = (unsigned int *)ray->img->data;
	wall->tex_col = wall->texture_slice * (double)ray->img->width;
	wall->true_line_height = wall->actual_bottom - wall->actual_top;
	wall->dist = ray->perp_wall_dist;
	if (wall->dist < 1.0)
		wall->dist = 1.0;
	wall->brightness = 1 / wall->dist;
}

void	paint_col(t_params *params, t_ray *ray, t_wall *wall, int px)
{
	int	ceiling_color;

	ceiling_color = params->cclr;
	if (params->lights)
		ceiling_color = brightness_adj(ceiling_color, wall->brightness);
	if (!(ray->next))
	{
		if (px < wall->top_of_wall)
			put_pixel(params, px, ray->col, ceiling_color);
		else if (px > wall->bottom_of_wall)
			put_pixel(params, px, ray->col, params->fclr);
	}
	if (px >= wall->top_of_wall && px <= wall->bottom_of_wall)
	{
		wall->row_slice = (double)(px - wall->actual_top)
			/ wall->true_line_height;
		wall->tex_row = wall->row_slice * (double)ray->img->height;
		wall->color = ray->img_data[(wall->tex_row * ray->img->size_line)
			/ (ray->img->bpp / 8) + wall->tex_col];
		if (params->lights)
			wall->color = brightness_adj(wall->color, wall->brightness);
		put_pixel(params, px, ray->col, wall->color);
	}
}

void	set_ray_img(t_params *params, t_ray *ray)
{
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
