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
            else
                for (int row = i * 100; row < (i + 1) * 100; row++)
					for (int col = j * 100; col < (j + 1) * 100; col++)
                        put_pixel(p, row, col, 0x000000);
	for (int row = 0; row <= SIZE; row++)
		for (int col = 0; col <= SIZE; col++)
			if (row % 100 == 0 || col % 100 == 0 || row == SIZE - 1 || col == SIZE - 1)
                put_pixel(p, row, col, 0xffffff);
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
	row = row / 6 * SIZE;
	col = col / 6 * SIZE;

	for (int j = col - 4; j < col + 4; j++)
		for (int i = row - 4; i < row + 4; i++)
			put_pixel(p, i, j, 0xffff0000);

	/* -------------------------------------------------------------------------- */
	/*                        DRAW LINE POINTING TO HEADING                       */
	/* -------------------------------------------------------------------------- */
	// get player coords
	// draw straight line up until row or col hits limit
	// int pixels = 0;
	
	while(((row >= 0 && row < SIZE) && (col >= 0 && col < SIZE))
	&& ((int) (row / SIZE * 6) == (int) player->position[0]) 
	&& ((int) (col / SIZE * 6) == (int) player->position[1]))
	{
		put_pixel(p, row, col, 0xffff00);
		// pixels++;
		row += -cos(player->heading);
		col += sin(player->heading);
	}
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
}


/** TODO:
 * 		- convert to singular rotation matrix
*/
void move_player(t_params *params, int direction)
{
	t_player *player = params->player;

	double step = 0.1;

	player->position[0] += cos(player->heading) * step * direction;
	player->position[1] += -sin(player->heading) * step * direction;
	printf("player pos: %f %f\n", player->position[0], player->position[1]);
}


void strafe_player(t_params *params, int direction)
{
	t_player *player = params->player;

	double step = 0.1;

	player->position[0] += sin(player->heading) * step * direction;
	player->position[1] += cos(player->heading) * step * direction;
}

double FOV = M_PI / 3; // degrees? 66? 60?
int WIDTH = 600; // how wide?

void draw_ray(t_params *p)
{
	t_player *player = p->player;

	for (int i = -WIDTH / 2; i < WIDTH / 2; i++)
	{
		double rayHeading = player->heading;
		rayHeading += (double) i / WIDTH * FOV;

		// printf("\t\t rayheading %f rad %f deg\n", rayHeading, rayHeading / M_PI * 360);
		double rayDirX = sin(player->heading + (double) i / WIDTH * FOV ) ;
		double rayDirY = -cos(player->heading + (double) i / WIDTH * FOV );

		double posX = player->position[1];
		double posY = player->position[0];

		int mapX = player->position[1];
		int mapY = player->position[0];

		double sideDistX;
		double sideDistY;

		double deltaDistX = (rayDirX == 0) ? INFINITY : fabs(1 / rayDirX);
		double deltaDistY = (rayDirY == 0) ? INFINITY : fabs(1 / rayDirY);
		double perpWallDist;

		int stepX;
		int stepY;

		int hit = 0;
		int side = 0;
		
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (1.0 + mapX - posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (1.0 + mapY - posY) * deltaDistY;
		}

		while(!hit)
		{
			if (sideDistX < sideDistY) //what if equal?
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (map[mapY][mapX] == 1)
			{
				hit = 1;
				// printf("player in grid %f, %f heading %f is looking at wall at %i %i\n", posX, posY, player->heading, mapX, mapY);
			}
		}
		double row = player->position[0]; // y
		double col = player->position[1]; // x

		if (side)
			perpWallDist = sideDistY - deltaDistY;
		else
			perpWallDist = sideDistX - deltaDistX;
		// multiply by width and height (scale??) to get pixel pos
		row = row / 6 * SIZE;
		col = col / 6 * SIZE;
		while(((row >= 0 && row < SIZE) && (col >= 0 && col < SIZE))
		&& ((side && (int) (row / SIZE * 6) != mapY)
			|| (!side && (int) (col / SIZE * 6) != mapX )))
		{
			put_pixel(*p, row, col, 0x00ff00);
			row += -cos(rayHeading);
			col += sin(rayHeading);
		}
	}
}

int	key_hook(int keycode, t_params *params)
{
	if (keycode == XK_Escape)
		return (close_window(params));
	else if (keycode == XK_w)
		move_player(params, -1);
	else if (keycode == XK_a)
		strafe_player(params, -1);
	else if (keycode == XK_s)
		move_player(params, 1);
	else if (keycode == XK_d)
		strafe_player(params, 1);
	else if (keycode == XK_Left)
		rotate_player(params, -3);
	else if (keycode == XK_Right)
		rotate_player(params, 3);
	else
		ft_printf("KEY: %i\n", keycode);
	// draw_ray(params);
	return (1);
}
