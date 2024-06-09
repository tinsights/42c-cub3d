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
    player->heading += M_PI / 180 * degrees;
    if (degrees > 0)
        printf("turning right, ");
    else
        printf("turning left, ");

    printf("%f %f\n", player->heading, player->heading / M_PI * 360);
    printf("sine of heading: %f\n", sin(player->heading));
    printf("cosine of heading: %f\n", cos(player->heading));
    printf("==============\n");
}

int centerOfScreen = WIN_HEIGHT / 2;
double playerHeight = 0.5;
double playerVertAngle = 0;

/** TODO:
 * 		- convert to singular rotation matrix
*/
void move_player(t_params *params, double direction)
{
	t_player *player = params->player;

	double horizStep = 0.1 * cos(playerVertAngle);
	double vertStep = 0.1 * -sin(playerVertAngle);

	printf("player vert angle: %f | vertStep: %f\n", playerVertAngle, vertStep);
	playerHeight += vertStep * direction;

	player->position[0] += cos(player->heading) * horizStep * direction;
	player->position[1] += -sin(player->heading) * horizStep * direction;
	printf("player pos: %f %f\n", player->position[0], player->position[1]);
}


void strafe_player(t_params *params, int direction)
{
	t_player *player = params->player;

	double step = 0.1;

	// double vertAngle = WIN_HEIGHT / 2 - centerOfScreen;

	player->position[0] += sin(player->heading) * step * direction;
	player->position[1] += cos(player->heading) * step * direction;
}

double FOV = (100.0 / 180.0 * M_PI); // degrees? 66? 60?



void draw_ray(t_params *p)
{
	t_player *player = p->player;

	double forwardX = sin(player->heading);
	double forwardY = -cos(player->heading);
	double projection_plane_width = 2.0 * tan(FOV / 2.0);
	// printf("ppw: %f\n", projection_plane_width);

	double rightX = -forwardY * projection_plane_width;
	double rightY = forwardX * projection_plane_width;

	for (int i = -WIN_WIDTH / 2; i < WIN_WIDTH / 2; i++)
	{
		double progress = ((double) i + WIN_WIDTH / 2) / WIN_WIDTH - 0.5; // goes from -0.5 to 0.5. can normalise to -1 to 1

		// double rayHeading = player->heading;
		// rayHeading += (double) i / WIN_WIDTH * FOV;
		// printf("\t\t rayheading %f rad %f deg\n", rayHeading, rayHeading / M_PI * 180);

		double rayDirX = forwardX + progress * rightX;
		double rayDirY = forwardY + progress * rightY;


		// double rayHeading = sqrt(rayDirX * rayDirX + rayDirY * rayDirY);

		double posX = player->position[1];
		double posY = player->position[0];

		int mapX = player->position[1];
		int mapY = player->position[0];

		double sideDistX;
		double sideDistY;

		double deltaDistX = (rayDirX == 0) ? INFINITY : fabs(1 / rayDirX );
		double deltaDistY = (rayDirY == 0) ? INFINITY : fabs(1 / rayDirY );
		double perpWallDist;

		int stepX;
		int stepY;

		int hit = 0;
		int side = 0;
		
		if (rayDirX < 0) // looking "west"
		{
			stepX = -1;
			sideDistX = (posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (1.0 + mapX - posX) * deltaDistX;
		}
		if (rayDirY < 0) // looking "north"
		{
			stepY = -1;
			sideDistY = (posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (1.0 + mapY - posY) * deltaDistY;
		}

		if (map[mapY][mapX] != 0)
		{
			if (sideDistX < sideDistY) // what if equal?
			{
				sideDistX += deltaDistX;
				side = 1;
			}
			else
			{
				sideDistY += deltaDistY;
				side = 0;
			}
			hit = 1;
			// if (sideDistX != sideDistY)
			// {
			// 	printf("player in grid %f, %f heading %f is looking at wall at %i %i ", posX, posY, player->heading / M_PI * 180, mapX, mapY);
			// 	if (side)
			// 		printf("from the sides\n");
			// 	else
			// 		printf("vertically\n");
			// 	printf("deltaDistX is %f deltaDistY is %f\n", deltaDistX, deltaDistY);
			// }
			// else
			// 	printf("equal sidedists!\n");
		}


		while(!hit)
		{
			if (sideDistX < sideDistY) // what if equal?
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 1;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 0;
			}
			if (map[mapY][mapX] != 0)
			{
				hit = 1;
				// if (sideDistX != sideDistY)
				// {
				// 	printf("player in grid %f, %f heading %f is looking at wall at %i %i ", posX, posY, player->heading / M_PI * 180, mapX, mapY);
				// 	if (side)
				// 		printf("from the sides\n");
				// 	else
				// 		printf("vertically\n");
				// 	printf("deltaDistX is %f deltaDistY is %f\n", deltaDistX, deltaDistY);
				// }
				// else
				// 	printf("equal sidedists!\n");
			}
		}



		if (!side)
			perpWallDist = sideDistY - deltaDistY;
		else
			perpWallDist = sideDistX - deltaDistX;
		
		// if (i == WIN_WIDTH / 2 - 1)
		// {
		// 	if (!side)
		// 		printf("Side Dist Y: %f, ddy: %f\n", sideDistY, deltaDistY);
		// 	else
		// 		printf("Side Dist X: %f, ddX: %f\n", sideDistX, deltaDistX);

		// 	printf("pwd before: %f\n", perpWallDist);
		// }

		// long double factor = fabs(cos((double) (i) * FOV / WIN_WIDTH));
		// perpWallDist *= factor; // - 30 to 30
		// if (i == 0)
		// 	printf("direct perp wall dist: %f\n", perpWallDist);
		// if (i == WIN_WIDTH / 2 - 1)
		// {
		// 	// printf("pwd before: %f\n", perpWallDist);
		// 	printf("FOV: %f %f\n", FOV, FOV * 180 / M_PI);
		// 	double angle = (double) (i + 1) * FOV / WIN_WIDTH;
		// 	printf("angle: %f %f\n", angle, angle / M_PI * 180);
		// 	// printf("factor: %Lf\n", factor);
		// 	printf("pwd after: %f\n", perpWallDist);
		// }
		/* -------------------------------------------------------------------------- */
		/*                         Drawing 2d Rays for minimap                        */
		/* -------------------------------------------------------------------------- */
		// double row = player->position[0]; // y
		// double col = player->position[1]; // x
		// // multiply by width and height (scale??) to get pixel pos
		// row = row / MHEIGHT * WIN_HEIGHT;
		// col = col / MWIDTH * WIN_WIDTH;
		// while(((row >= 0 && row < WIN_HEIGHT) && (col >= 0 && col < WIN_WIDTH))
		// && ((!side && (int) (row / WIN_HEIGHT * MHEIGHT) != mapY)
		// 	|| (side && (int) (col / WIN_WIDTH * MWIDTH) != mapX )))
		// {
		// 	put_pixel(*p, row, col, 0x00ff00);
		// 	row += -cos(rayHeading);
		// 	col += sin(rayHeading);
		// }

		double distToProjectionPlane = (WIN_WIDTH / 2.0) / (tan(FOV / 2.0));
		
		double ratio = distToProjectionPlane / perpWallDist;

		double verticalShear = tan(playerVertAngle) * distToProjectionPlane;
		// double unit_height = (double)1; // a wall 1 unit grid away will take up 80% of my screen height
		// double lineHeight = (double)unit_height / perpWallDist;

		int trueBottomOfWall = ratio * playerHeight + centerOfScreen + verticalShear;
		int trueTopOfWall = trueBottomOfWall - ratio;

		int bottomOfWall = trueBottomOfWall;
		if (bottomOfWall >= WIN_HEIGHT)
			bottomOfWall = WIN_HEIGHT - 1;
		int topOfWall = trueTopOfWall;
		if (topOfWall < 0)
			topOfWall = 0;

		// printf("unit height is %i, perpWallDist is %f\n", unit_height, perpWallDist);
		// printf("lineheight is %i, bottom of wall is %i, top of wall is %i\n", lineHeight, bottomOfWall, topOfWall);
		int color = map[mapY][mapX] == 1 ? 0x0000ff : 0x880000;
		if (side)
			color /= 2;
		
		double flashAngle = perpWallDist + cos((double) (i / WIN_WIDTH * FOV));
		if (flashAngle > 8)
			color /= 2;
		else if (flashAngle > 12)
			color /= 4;

			

		for (int px = 0; px < WIN_HEIGHT; px++)
		{
			// color = color *(1 - ((double) px / (trueBottomOfWall)))
			if (px < topOfWall )
				put_pixel(*p, px, i + WIN_WIDTH / 2, 0x888888);
			else if (px> bottomOfWall )
				put_pixel(*p, px, i + WIN_WIDTH / 2, 0x333333);
			else
				put_pixel(*p, px, i + WIN_WIDTH / 2, color);
		}
	}
}

int	key_hook(int keycode, t_params *params)
{

	static double speed = 1.0;

	if (keycode == XK_Escape)
		return (close_window(params));
	else if (keycode == XK_w)
	{
		// if (speed < 4)
		// {
		// 	speed = (4.0 - speed) * 0.1;
		// }
		move_player(params, -speed);
	}
	else if (keycode == XK_a)
		strafe_player(params, -1);
	else if (keycode == XK_s)
		move_player(params, speed);
	else if (keycode == XK_d)
		strafe_player(params, 1);
	else if (keycode == XK_Left)
		rotate_player(params, -6);
	else if (keycode == XK_Right)
		rotate_player(params, 6);
	else if (keycode == 65451)
		FOV += M_PI / 30;
	else if (keycode == 65453)
		FOV -= M_PI / 30;
	else if (keycode == XK_Up)
	{
		playerVertAngle += M_PI / 60; // 12 degs
	}
	else if (keycode == XK_Down)
	{
		playerVertAngle -= M_PI / 60; // 12degs
	}
	else if (keycode == XK_Control_L)
	{
		playerHeight -= 0.25;
		// centerOfScreen += WIN_HEIGHT / 20;	
	}
	else if (keycode == XK_space)
	{
		playerHeight += 0.25;
		// centerOfScreen -= WIN_HEIGHT / 20;
	}
	else
		ft_printf("KEY: %i\n", keycode);
	// draw_ray(params);
	render(params);
	return (1);
}
