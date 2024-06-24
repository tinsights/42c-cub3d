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

void put_pixel(t_params p, t_uint row, t_uint col, int colour)
{
    char *px = p.mlx->img_addr + row * p.mlx->line_sz + col * (p.mlx->bpp / 8);
    *(t_uint *) px = mlx_get_color_value(p.mlx->ptr, colour);
}
int	close_window(t_params *params)
{
	mlx_destroy_image(params->mlx->ptr, params->mlx->img);
	mlx_destroy_image(params->mlx->ptr, params->north);
	mlx_destroy_image(params->mlx->ptr, params->east);
	mlx_destroy_image(params->mlx->ptr, params->west);
	mlx_destroy_image(params->mlx->ptr, params->south);
	mlx_destroy_image(params->mlx->ptr, params->inner);

	mlx_destroy_window(params->mlx->ptr, params->mlx->win);
	mlx_destroy_display(params->mlx->ptr);
	free(params->mlx->ptr);
	exit(1);
	return (1);
}

void rotate_player(t_params *params, float degrees)
{
    t_player *player = params->player;
    player->heading += M_PI / 180 * degrees;
    // if (degrees > 0)
    //     printf("turning right, ");
    // else
    //     printf("turning left, ");

    // printf("%f %f\n", player->heading, player->heading / M_PI * 360);
    // printf("sine of heading: %f\n", sin(player->heading));
    // printf("cosine of heading: %f\n", cos(player->heading));
    // printf("==============\n");
}

/** TODO:
 * 		- convert to singular rotation matrix
*/

#define STEP 0.02

// bool precision_wall_check(char** map, double x, double y)
// {
// 	int integer;
// 	double xfract = modf(x, &integer);
// 	double yfract = modf(y, &integer);
// 	if (xfract > 0.9 || xfract < 0.1)
// 		x = round(x);
// 	if (yfract > 0.9 || yfract < 0.1)
// 		y = round(y);

// 	return (is_wall(map[(int) y][(int) x]));
// }
bool explorable(t_params *params, double y, double x)
{
	if (x < 0 || x > params->mwidth || y < 0 || y > params->mheight)
		return false;
	// if (params->map[(int) y][(int) x] == 'a')
	// 	return false;
	return (params->player->god || !is_wall(params->map[(int)y][(int)x]));
		
}

void move_player(t_params *params, float direction)
{
	t_player *player = params->player;

	float  horizStep = STEP * cos(player->vert_angle);
	float  vertStep = STEP * -sin(player->vert_angle);

	// printf("player vert angle: %f | vertStep: %f\n", player->vert_angle, vertStep);
	float  new_height = player->height + vertStep * direction;
	if (player->god)
		player->height = new_height;

	float  curr_y = player->position[0];
	float  curr_x = player->position[1];
	float  heading = player->heading;
	float  new_y = curr_y + cos(heading) * horizStep * direction;
	float  new_x = curr_x + -sin(heading) * horizStep * direction;

	// printf("new x %f %i new y %f %i\n", new_x, (int) new_x, new_y, (int) new_y);
	printf("speed: %f\n", direction);

	if (explorable(params, curr_y, new_x))
	{
			player->position[1] = new_x;
			curr_x = new_x;
	}
	if (explorable(params, new_y, curr_x))
	{
		if (new_y > 0 && new_y < params->mheight)
			player->position[0] = new_y;
	}

}

void strafe_player(t_params *params, float direction)
{
	t_player *player = params->player;

	float curr_y = player->position[0];
	float curr_x = player->position[1];
	float heading = player->heading;

	float new_y = curr_y + sin(heading) * STEP * direction;
	float new_x = curr_x + cos(heading) * STEP * direction;
	// printf("new x %f %i new y %f %i\n", new_x, (int) new_x, new_y, (int) new_y);



	if (explorable(params, curr_y, new_x))
	{
			player->position[1] = new_x;
			curr_x = new_x;
	}
	if (explorable(params, new_y, curr_x))
	{
		if (new_y > 0 && new_y < params->mheight)
			player->position[0] = new_y;
	}
}

void spraypaint(t_params *params)
{
	t_player *player = params->player;

	double heading = player->heading;

	int map_x = player->position[1] + sin(heading);
	int map_y = player->position[0] - cos(heading);
		
	if (params->map[map_y][map_x] == '1')
		params->map[map_y][map_x] = 't';
	else if (params->map[map_y][map_x] == 't')
		params->map[map_y][map_x] = '1';
	else if (params->map[map_y][map_x] == '2')
		params->map[map_y][map_x] = 'T';
	else if (params->map[map_y][map_x] == 'T')
		params->map[map_y][map_x] = '2';
}

void door(t_params *params)
{
	t_player *player = params->player;

	double heading = player->heading;

	int map_x = player->position[1] +  sin(heading);
	int map_y = player->position[0] - cos(heading);
		
	if (params->map[map_y][map_x] == 'D')
		params->map[map_y][map_x] = 'd';
	else if (params->map[map_y][map_x] == 'd')
		params->map[map_y][map_x] = 'D';
}

int	key_hook(int keycode, t_params *params)
{
	t_player *player = params->player;

	if (keycode == XK_Escape)
		return (close_window(params));
	else if (keycode == XK_w)
		params->player->move_ws = -1;
	else if (keycode == XK_a)
		params->player->move_ad = -1;
	else if (keycode == XK_s)
		params->player->move_ws = 1;
	else if (keycode == XK_d)
		params->player->move_ad = 1;
	else if (keycode == XK_Left)
		params->player->move_turn = -1;
	else if (keycode == XK_Right)
		params->player->move_turn = 1;
	else if (keycode == XK_KP_Add)
		params->fov += M_PI / 30;
	else if (keycode == XK_KP_Subtract)
		params->fov -= M_PI / 30;
	else if (keycode == XK_Up)
		params->player->move_tilt = 1;
	else if (keycode == XK_Down)
		params->player->move_tilt = -1;
	else if (keycode == XK_Control_L)
	{
		if (player->god || player->height > 0.25)
			player->height -= 0.25;
	}
	else if (keycode == XK_space)
	{
		if (player->god || player->height < 0.75)
			player->height += 0.25;
	}
	else if (keycode == XK_t)
		spraypaint(params);
	else if (keycode == XK_e)
		door(params);
	else if (keycode == XK_g)
		player->god = !player->god;
	else
		ft_printf("KEY: %i\n", keycode);
	// render(params);
	return (1);
}
int	key_release_hook(int keycode, t_params *params)
{
	if (keycode == XK_w)
		params->player->move_ws = 0;
	else if (keycode == XK_a)
		params->player->move_ad = 0;
	else if (keycode == XK_s)
		params->player->move_ws = 0;
	else if (keycode == XK_d)
		params->player->move_ad = 0;
	else if (keycode == XK_Left)
		params->player->move_turn = 0;
	else if (keycode == XK_Right)
		params->player->move_turn = 0;
	else if (keycode == XK_Up)
		params->player->move_tilt = 0;
	else if (keycode == XK_Down)
		params->player->move_tilt = 0;
	return (1);
}


void build_wall(t_params *params)
{
	t_player *player = params->player;

	double heading = player->heading;


	int map_x = player->position[1] + sin(heading);
	int map_y = player->position[0] - cos(heading);

	printf("%i %i %f %f\n", map_x, map_y, sin(heading), -cos(heading));
	char grid = params->map[map_y][map_x];
	if (grid == '0')
	{
		map_x += sin(heading);
		map_y -= cos(heading);
	}
		
	if (params->map[map_y][map_x] == '0')
		params->map[map_y][map_x] = '2';
	else if (params->map[map_y][map_x] == '2')
		params->map[map_y][map_x] = '0';
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
	}
	else if (button == 5)
	{
		params->fov += M_PI / 30;
		printf("zooming out\n");
	}
	else if (button == 3)
	{
		build_wall(params);
	}
	// render(params);
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
	// render(params);

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
