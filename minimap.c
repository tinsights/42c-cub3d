/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:10:59 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 14:11:00 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	paint_grid(t_params *p, t_minimap *mm);
void	paint_block(t_params *p, t_minimap *mm);
void	paint_player(t_params *p, t_minimap *mm);
bool	unbounded(t_params *p, t_minimap *mm);
bool	hit_wall(t_params *p, t_minimap *mm);

void	draw_rays(t_params *p, t_minimap *mm)
{
	double	projection_plane_x;
	int		ray;

	ray = -WIN_WIDTH / 2;
	while (ray < WIN_WIDTH / 2)
	{
		projection_plane_x = 2.0 * (double)ray / (WIN_WIDTH - 1);
		mm->dir_x = mm->heading_x + projection_plane_x * mm->plane_x;
		mm->dir_y = mm->heading_y + projection_plane_x * mm->plane_y;
		mm->heading_px_row = mm->total_size / 2;
		mm->heading_px_col = mm->total_size / 2;
		while (mm->heading_px_col < mm->total_size && mm->heading_px_col > 0
			&& mm->heading_px_row > 0 && mm->heading_px_row < mm->total_size)
		{
			if (hit_wall(p, mm))
				break ;
			put_pixel(p, mm->heading_px_row, mm->heading_px_col, 0x550077);
			mm->heading_px_row += mm->dir_y;
			mm->heading_px_col += mm->dir_x;
		}
		ray++;
	}
}

void	fill_grid(t_params *p, t_minimap *mm)
{
	mm->px_col = -1;
	while (++mm->px_col < mm->total_size)
	{
		mm->px_row = -1;
		while (++mm->px_row < mm->total_size)
		{
			paint_grid(p, mm);
			if (unbounded(p, mm))
				continue ;
			paint_block(p, mm);
			paint_player(p, mm);
		}
	}
}

void	init_minimap(t_params *p, t_minimap *mm)
{
	mm->sq_size = 15;
	mm->mm_size = 8;
	mm->total_size = mm->sq_size * mm->mm_size;
	mm->heading = p->player.heading;
	mm->pos_y = p->player.position[0];
	mm->pos_x = p->player.position[1];
	mm->off_y = (p->player.position[0] - mm->pos_y) * mm->sq_size;
	mm->off_x = (p->player.position[1] - mm->pos_x) * mm->sq_size;
	mm->half_projection_plane_width = tan(p->fov / 2.0);
	mm->heading_x = sin(mm->heading);
	mm->heading_y = -cos(mm->heading);
	mm->plane_x = -mm->heading_y * mm->half_projection_plane_width;
	mm->plane_y = mm->heading_x * mm->half_projection_plane_width;
}

void	paint_mm(t_params *p, t_minimap *mm)
{
	mm->px_col = -1;
	while (++mm->px_col < mm->total_size)
	{
		mm->px_row = -1;
		while (++mm->px_row < mm->total_size)
			put_pixel(p, mm->px_row, mm->px_col, 0xa9a9a9);
	}
}

void	draw_minimap(t_params *p)
{
	t_minimap	mm;

	init_minimap(p, &mm);
	paint_mm(p, &mm);
	draw_rays(p, &mm);
	fill_grid(p, &mm);
}