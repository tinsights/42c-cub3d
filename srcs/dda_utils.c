/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:41:15 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 15:41:16 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycast.h"

void	check_special(t_params *params, t_ray *ray)
{
	if (params->map[ray->map_y][ray->map_x] == 't'
		|| params->map[ray->map_y][ray->map_x] == 'T')
	{
		if (params->spray)
			ray->img = params->spray;
		ray->type = spray;
	}
	else if (params->map[ray->map_y][ray->map_x] == 'D')
	{
		if (params->door)
			ray->img = params->door;
		ray->type = door;
	}
	if (params->map[ray->map_y][ray->map_x] == 'd')
		ray->hit = false;
}

void	check_if_wall(t_params *params, t_ray *ray)
{
	if (ray->map_y < 0 || ray->map_y >= params->mheight
		|| ray->map_x < 0 || ray->map_x >= params->mwidth
		|| params->map[ray->map_y][ray->map_x] != '0')
	{
		ray->hit = true;
		set_ray_img(params, ray);
	}
	ray->type = wall;
	if ((ray->map_y > 0 && ray->map_y < params->mheight
		&& ray->map_x > 0 && ray->map_x < params->mwidth))
		check_special(params, ray);
}

void	cast_to_wall(t_params *params, t_ray *ray)
{
	while (!ray->hit)
	{
		increment_ray(ray);
		check_if_wall(params, ray);
	}
	if (ray->side_x)
		ray->perp_wall_dist = ray->dist_x - ray->delta_dist_x;
	else
		ray->perp_wall_dist = ray->dist_y - ray->delta_dist_y;
}

void	extend_ray(t_ray *ray, t_ray *next)
{
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
}

void	ray_pierce(t_params *params, t_ray *ray)
{
	t_ray	*next;

	if ((ray->map_x > 0 && ray->map_x < params->mwidth - 1) && (ray->map_y > 0
			&& ray->map_y < params->mheight - 1))
	{
		next = ft_calloc(1, sizeof(t_ray));
		extend_ray(ray, next);
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
