/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2d.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 10:20:41 by tjegades          #+#    #+#             */
/*   Updated: 2024/06/10 10:20:42 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define GRID_HEIGHT WIN_HEIGHT / MHEIGHT
#define GRID_WIDTH WIN_WIDTH / MWIDTH

void draw_grid(t_params p)
{
	for (int i = 0; i < MHEIGHT; i++)
		for (int j = 0; j < MWIDTH; j++)
			if (map[i][j] == 1)
				for (int row = i * GRID_HEIGHT; row < (i + 1) * GRID_HEIGHT; row++)
					for (int col = j * GRID_WIDTH; col < (j + 1) * GRID_WIDTH; col++)
					{
						if (row == i * GRID_HEIGHT || col == j * GRID_WIDTH)
			                put_pixel(p, row, col, 0xffffff);
						else
                        	put_pixel(p, row, col, 0x888888);
					}
            else
                for (int row = i * GRID_HEIGHT; row < (i + 1) * GRID_HEIGHT; row++)
					for (int col = j * GRID_WIDTH; col < (j + 1) * GRID_WIDTH; col++)
					{
						if (row == i * GRID_HEIGHT || col == j * GRID_WIDTH)
			                put_pixel(p, row, col, 0xffffff);
						else
                        	put_pixel(p, row, col, 0x000000);
					}
	// for (int row = 0; row <= SIZE; row++)
	// 	for (int col = 0; col <= SIZE; col++)
	// 		if (row % 85 == 0 || col % 85 == 0 || row == SIZE - 1 || col == SIZE - 1)
    //             put_pixel(p, row, col, 0xffffff);
}


void draw_player(t_params p)
{
	t_player *player = p.player;

	// get gridsq of player
	// this is some f-point number that is bounded by the grid
	// i.e. [0,0] to say, [6,6]
	double row = player->position[0]; // y
	double col = player->position[1]; // x

	// multiply by width and height (scale??) to get pixel pos
	row = row / MHEIGHT * WIN_HEIGHT;
	col = col / MWIDTH * WIN_WIDTH;

	for (int j = col - 4; j < col + 4; j++)
		for (int i = row - 4; i < row + 4; i++)
			put_pixel(p, i, j, 0xffff0000);

	/* -------------------------------------------------------------------------- */
	/*                        DRAW LINE POINTING TO HEADING                       */
	/* -------------------------------------------------------------------------- */
	// get player coords
	// draw straight line up until row or col hits limit
	// int pixels = 0;
	
	while(((row >= 0 && row < WIN_HEIGHT) && (col >= 0 && col < WIN_WIDTH))
	&& ((int) (row / WIN_HEIGHT * MHEIGHT) == (int) player->position[0]) 
	&& ((int) (col / WIN_WIDTH * MWIDTH) == (int) player->position[1]))
	{
		put_pixel(p, row, col, 0xffff00);
		// pixels++;
		row += -cos(player->heading);
		col += sin(player->heading);
	}
}
