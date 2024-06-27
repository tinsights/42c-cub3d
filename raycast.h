/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:44:15 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 15:44:16 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_H
# define RAYCAST_H

# include "cub3d.h"

void	reset_ray(t_ray *ray, t_player *player);
void	direct_ray(t_ray *ray, t_player *player);
void	dda(t_params *params, t_ray *ray);
void	paint_walls(t_params *params, t_player *player, t_ray *ray);
void	draw_walls(t_params *params);
bool	is_wall(char c);
void	inner_wall(t_params *params, t_ray *ray);
void	increment_ray(t_ray *ray);
void	check_special(t_params *params, t_ray *ray);
void	check_if_wall(t_params *params, t_ray *ray);
void	cast_to_wall(t_params *params, t_ray *ray);
void	extend_ray(t_ray *ray, t_ray *next);
void	ray_pierce(t_params *params, t_ray *ray);
void	set_ray_img(t_params *params, t_ray *ray);
void	paint_col(t_params *params, t_ray *ray, t_wall *wall, int px);
void	get_wall_dimensions(t_params *params, t_player *player, t_ray *ray,
			t_wall *wall);
void	get_texture(t_ray *ray, t_wall *wall);
int		brightness_adj(int col, float brightness);

#endif
