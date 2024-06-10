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
int	close_window(t_params *params)
{
	mlx_destroy_image(params->mlx->ptr, params->mlx->img);
	mlx_destroy_window(params->mlx->ptr, params->mlx->win);
	mlx_destroy_display(params->mlx->ptr);
	free(params->mlx->ptr);
	exit(1);
	return (1);
}

void rotate_player(t_params *params, int degrees)
{
    t_player *player = params->player;
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

/** TODO:
 * 		- convert to singular rotation matrix
*/
void move_player(t_params *params, double direction)
{
	t_player *player = params->player;

	double horizStep = 0.1 * cos(player->vert_angle);
	double vertStep = 0.1 * -sin(player->vert_angle);

	printf("player vert angle: %f | vertStep: %f\n", player->vert_angle, vertStep);
	player->height += vertStep * direction;

	player->position[0] += cos(player->heading) * horizStep * direction;
	player->position[1] += -sin(player->heading) * horizStep * direction;
	printf("player pos: %f %f\n", player->position[0], player->position[1]);
}


void strafe_player(t_params *params, int direction)
{
	t_player *player = params->player;

	double step = 0.1;

	player->position[0] += sin(player->heading) * step * direction;
	player->position[1] += cos(player->heading) * step * direction;
	printf("player pos: %f %f\n", player->position[0], player->position[1]);
}

int	key_hook(int keycode, t_params *params)
{
	t_player *player = params->player;

	if (keycode == XK_Escape)
		return (close_window(params));
	else if (keycode == XK_w)
		move_player(params, -player->speed);
	else if (keycode == XK_a)
		strafe_player(params, -player->speed);
	else if (keycode == XK_s)
		move_player(params, player->speed);
	else if (keycode == XK_d)
		strafe_player(params, player->speed);
	else if (keycode == XK_Left)
		rotate_player(params, -6);
	else if (keycode == XK_Right)
		rotate_player(params, 6);
	else if (keycode == XK_KP_Add)
		params->fov += M_PI / 30;
	else if (keycode == XK_KP_Subtract)
		params->fov -= M_PI / 30;
	else if (keycode == XK_Up)
		player->vert_angle += M_PI / 60; // 12 degs
	else if (keycode == XK_Down)
		player->vert_angle -= M_PI / 60; // 12degs
	else if (keycode == XK_Control_L)
		player->height -= 0.25;
	else if (keycode == XK_space)
		player->height += 0.25;
	else
		ft_printf("KEY: %i\n", keycode);
	render(params);
	return (1);
}
int mouse_click(int button, int x, int y, t_params *params)
{
	printf("button: %i | mouseX: %i | mouseY: %i\n", button, x, y);

	if (button == 2)
	{
		params->clicked_px[0] = x;
		params->clicked_px[1] = y;
	}
	else if (button == 4)
	{
		params->fov -= M_PI / 30;
		printf("zooming in\n");
		render(params);
	}
	else if (button == 5)
	{
		params->fov += M_PI / 30;
		printf("zooming out\n");
		render(params);
	}
	return (1);
}

int mouse_move(int x, int y, t_params *params)
{
	t_player *player = params->player;

	int xDelta = x - params->clicked_px[0];
	int yDelta = y - params->clicked_px[1];

	// double projection_plane_width = 2.0 * tan(params->fov / 2.0);

	//2∗atan(tan(h/2)∗AR)
	// printf("FOV: %f\n", params->fov);

	// double ratio = tan(params->fov / 2) * 1.6;
	// printf("ratio: %f\n", ratio);
	// double vertFov = 2.0 * atan(ratio);
	// printf("vertFov: %f\n", vertFov);

	double heading_delta = (double) xDelta / WIN_WIDTH * tan(params->fov / 2.0);
	double vert_delta = (double) yDelta / WIN_WIDTH * tan(params->fov / 2.0);
	params->player->heading += 4.0 * heading_delta;

	player->vert_angle -= 4.0 * vert_delta;
	// printf(" mouseX: %i | mouseY: %i | xDelta: %i | ydelta: %i | heading_delta: %f vert_delta: %f\n",  x, y, xDelta, yDelta, heading_delta, vert_delta);
	params->clicked_px[0] = x;
	params->clicked_px[1] = y;
	render(params);

	return (1);
}


int key_release_hook(int keycode, t_params *params)
{
	printf("keycode: %i\n", keycode);
	if (keycode == XK_w && params)
	{
		printf("hello\n");
	}
	return (1);
}

void draw_crosshair(t_params p)
{
	int vert_center = WIN_HEIGHT / 2;
	int horiz_center = WIN_WIDTH / 2;

	int half_crosshair_length = 8;

	for (int i = vert_center - half_crosshair_length; i < vert_center + half_crosshair_length; i++)
		put_pixel(p, i, horiz_center, 0x888800);
	for (int i = horiz_center - half_crosshair_length; i < horiz_center + half_crosshair_length; i++)
		put_pixel(p, vert_center, i, 0x888800);
}