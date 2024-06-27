/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:23:19 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/27 14:23:19 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	hit_wall(t_params *p, t_minimap *mm)
{
	mm->col_check = mm->pos_x - mm->mm_size / 2 + (mm->heading_px_col
			+ mm->off_x) / mm->sq_size;
	mm->row_check = mm->pos_y - mm->mm_size / 2 + (mm->heading_px_row
			+ mm->off_y) / mm->sq_size;
	if (mm->col_check < 0 || mm->row_check < 0 || mm->col_check >= p->mwidth
		|| mm->row_check >= p->mheight)
		return (true);
	mm->block = p->map[mm->row_check][mm->col_check];
	if (mm->block != '0' && mm->block != 'd' && (p->lights
			|| (p->player.height <= 1.1 && p->player.height >= -0.1)))
		return (true);
	return (false);
}

void	paint_player(t_params *p, t_minimap *mm)
{
	if (mm->px_col > mm->total_size / 2 - 2 && mm->px_col < mm->total_size / 2
		+ 2 && mm->px_row > mm->total_size / 2 - 2
		&& mm->px_row < mm->total_size / 2 + 2)
		put_pixel(p, mm->px_row, mm->px_col, 0x00ffff);
}

void	paint_block(t_params *p, t_minimap *mm)
{
	mm->block = p->map[mm->row_check][mm->col_check];
	if (mm->block == 'a')
		put_pixel(p, mm->px_row, mm->px_col, 0x880000);
	if (mm->block != '0' && mm->block != 'd')
		put_pixel(p, mm->px_row, mm->px_col, 0xff0000);
}

bool	unbounded(t_params *p, t_minimap *mm)
{
	mm->col_check = mm->pos_x - mm->mm_size / 2 + (mm->px_col + mm->off_x)
		/ mm->sq_size;
	mm->row_check = mm->pos_y - mm->mm_size / 2 + (mm->px_row + mm->off_y)
		/ mm->sq_size;
	if (mm->col_check < 0 || mm->row_check < 0 || mm->col_check >= p->mwidth
		|| mm->row_check >= p->mheight)
	{
		put_pixel(p, mm->px_row, mm->px_col, 0x111111);
		return (true);
	}
	return (false);
}

void	paint_grid(t_params *p, t_minimap *mm)
{
	if (mm->px_row == 0 || mm->px_row == mm->total_size - 1
		|| mm->px_col == 0 | mm->px_col == mm->total_size - 1)
		put_pixel(p, mm->px_row, mm->px_col, 0x000000);
	else if ((mm->px_row + mm->off_y) % mm->sq_size == 0 || (mm->px_col
			+ mm->off_x) % mm->sq_size == 0)
		put_pixel(p, mm->px_row, mm->px_col, 0xffffff);
}
