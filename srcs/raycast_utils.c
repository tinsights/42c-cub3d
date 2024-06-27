/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:36:55 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 15:36:56 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycast.h"

bool	is_wall(char c)
{
	return (c == '1' || c == '2'
		|| c == 'D' || c == 'a'
		|| c == 't' || c == 'T');
}

void	inner_wall(t_params *params, t_ray *ray)
{
	if (is_wall(params->map[ray->map_y][ray->map_x]))
	{
		ray->hit = true;
		if (params->inner)
			ray->img = params->inner;
		else
			set_ray_img(params, ray);
		if (ray->dist_x < ray->dist_y)
		{
			ray->dist_x += ray->delta_dist_x;
			ray->side_x = 1;
		}
		else
		{
			ray->dist_y += ray->delta_dist_y;
			ray->side_x = 0;
		}
	}
}

void	increment_ray(t_ray *ray)
{
	if (ray->dist_x < ray->dist_y)
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
}
