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

#include "raycast.h"

void	reset_ray(t_ray *ray, t_player *player)
{
	ray->id = 1;
	ray->projection_plane_x = 2.0 * ((double)ray->col) / (WIN_WIDTH - 1) - 1;
	ray->dir_x = ray->heading_x + ray->projection_plane_x * ray->plane_x;
	ray->dir_y = ray->heading_y + ray->projection_plane_x * ray->plane_y;
	if (ray->dir_x == 0)
		ray->delta_dist_x = INFINITY;
	else
		ray->delta_dist_x = fabs(1 / ray->dir_x);
	if (ray->dir_y == 0)
		ray->delta_dist_y = INFINITY;
	else
		ray->delta_dist_y = fabs(1 / ray->dir_y);
	ray->map_x = (int)player->position[1];
	ray->map_y = (int)player->position[0];
	ray->hit = false;
	ray->side_x = false;
	ray->perp_wall_dist = 0.0;
	ray->step_x = 0;
	ray->step_y = 0;
	ray->dist_x = 0.0;
	ray->dist_y = 0.0;
	ray->next = NULL;
}

void	direct_ray(t_ray *ray, t_player *player)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->dist_x = (player->position[1] - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->dist_x = (1.0 + ray->map_x - player->position[1])
			* ray->delta_dist_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->dist_y = (player->position[0] - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->dist_y = (1.0 + ray->map_y - player->position[0])
			* ray->delta_dist_y;
	}
}

void	dda(t_params *params, t_ray *ray)
{
	inner_wall(params, ray);
	cast_to_wall(params, ray);
	if (!params->lights && (params->player.height > 1.0
			|| params->player.height < 0))
		ray_pierce(params, ray);
}

void	paint_walls(t_params *params, t_player *player, t_ray *ray)
{
	t_wall	wall;
	int		px;

	if (ray->next)
	{
		paint_walls(params, player, ray->next);
		free(ray->next);
	}
	get_wall_dimensions(params, player, ray, &wall);
	get_texture(ray, &wall);
	px = -1;
	while (++px < WIN_HEIGHT)
		paint_col(params, ray, &wall, px);
}

void	draw_walls(t_params *params)
{
	t_player	*player;
	t_ray		ray;
	int			col;

	player = &(params->player);
	ray.heading_x = sin(player->heading);
	ray.heading_y = -cos(player->heading);
	params->half_plane_width = tan(params->fov / 2.0);
	ray.plane_x = -ray.heading_y * params->half_plane_width;
	ray.plane_y = ray.heading_x * params->half_plane_width;
	col = -1;
	while (++col < WIN_WIDTH)
	{
		ray.col = col;
		reset_ray(&ray, player);
		direct_ray(&ray, player);
		dda(params, &ray);
		paint_walls(params, player, &ray);
	}
}
