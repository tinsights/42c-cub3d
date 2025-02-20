/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <tjegades@student.42singapore.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:27:25 by tjegades          #+#    #+#             */
/*   Updated: 2025/02/20 20:48:49 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_walls(t_params *params);
void	move(t_params *params);

void	put_pixel(t_params *p, t_uint row, t_uint col, int colour)
{
	char	*px;

	px = p->mlx.img_addr + row * p->mlx.line_sz + col * (p->mlx.bpp / 8);
	*(t_uint *)px = mlx_get_color_value(p->mlx.ptr, colour);
}

void	draw_crosshair(t_params *p)
{
	int	vert_center;
	int	horiz_center;
	int	half_crosshair_length;
	int	i;

	vert_center = WIN_HEIGHT / 2;
	horiz_center = WIN_WIDTH / 2;
	half_crosshair_length = 8;
	i = vert_center - half_crosshair_length;
	while (i < vert_center + half_crosshair_length)
		put_pixel(p, i++, horiz_center, 0x888800);
	i = horiz_center - half_crosshair_length;
	while (i < horiz_center + half_crosshair_length)
		put_pixel(p, vert_center, i++, 0x888800);
}

int	render(t_params *p)
{
	move(p);
	draw_walls(p);
	if (WIN_HEIGHT > 8 && WIN_WIDTH > 8)
		draw_crosshair(p);
	if (MM_SQ_SZ * MM_GRID_SZ < WIN_HEIGHT && MM_SQ_SZ * MM_GRID_SZ < WIN_WIDTH)
		draw_minimap(p);
	mlx_put_image_to_window(p->mlx.ptr, p->mlx.win, p->mlx.img, 0, 0);
	return (1);
}
