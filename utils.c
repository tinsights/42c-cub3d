/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:09:43 by tjegades          #+#    #+#             */
/*   Updated: 2024/05/21 13:09:45 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#include <math.h>

void put_pixel(t_params p, t_uint row, t_uint col, int colour)
{
    char *px = p.mlx->img_addr + row * p.mlx->line_sz + col * (p.mlx->bpp / 8);
    *(t_uint *) px = mlx_get_color_value(p.mlx->ptr, colour);
}

void draw_grid(t_params p)
{
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
			if (map[i][j] == 1)
				for (int row = i * 100; row < (i + 1) * 100; row++)
					for (int col = j * 100; col < (j + 1) * 100; col++)
                        put_pixel(p, row, col, 0x888888);
	for (int row = 0; row <= SIZE; row++)
		for (int col = 0; col <= SIZE; col++)
			if (row % 100 == 0 || col % 100 == 0 || row == SIZE - 1 || col == SIZE - 1)
                put_pixel(p, row, col, 0xffffff);
}

void draw_player(t_params p)
{
	t_player *player = p.player;
	int pl_col = player->px_location[0] - 4;
	int pl_row = player->px_location[1] - 4;

	for (int row = pl_row; row < pl_row + 8; row++)
		for (int col = pl_col; col < pl_col + 8; col++)
            put_pixel(p, row, col, 0xff0000);

}
int	close_window(t_params *params)
{
	mlx_destroy_image(params->mlx->ptr, params->mlx->img);
	mlx_destroy_window(params->mlx->ptr, params->mlx->win);
	mlx_destroy_display(params->mlx->ptr);
	free(params->mlx->ptr);
	exit(1);
	return (1);
}

void rotate_player(t_params *p, int degrees)
{
    t_player *player = p->player;
    player->heading += M_PI / 360 * degrees;
    if (degrees > 0)
        printf("turning right, ");
    else
        printf("turning left, ");

    printf("%f %f\n", player->heading, player->heading / M_PI * 360);
    printf("sine of heading: %f\n", sin(player->heading));
    printf("cosine of heading: %f\n", cos(player->heading));
    printf("==============\n");
    	/* -------------------------------------------------------------------------- */
	/*                        DRAW LINE POINTING TO HEADING                       */
	/* -------------------------------------------------------------------------- */
	// get player coords
	// draw straight line up until row or col hits limit
	int player_x = player->px_location[0]; // should probably redo this to specify 
	int player_y = player->px_location[1]; // player_x and player_y in struct, for clarity


	// // take into acount heading

	// for (int row = player_y; row >= 0; row--)
	// 	put_pixel(*p,row, player_x, 0xffff00);

	double row = player_y;
	double col = player_x;
	while ((row >= 0 && row < SIZE) && (col >= 0 && col < SIZE))
	{
		printf("***************\n");
		printf("row: %f || col: %f\n", row, col);
		printf("***************\n");
		put_pixel(*p, row, col, 0xffff00);
		// row--;
		row += -cos(player->heading);
		col += sin(player->heading);
	}
}

int	key_hook(int keycode, t_params *params)
{
	if (keycode == XK_Escape)
		return (close_window(params));
	else if (keycode == XK_Left)
	{
		rotate_player(params, -2);
	}
	else if (keycode == XK_Right)
	{
		rotate_player(params, 2);

	}
	else
		ft_printf("KEY: %i\n", keycode);
	return (1);
}
