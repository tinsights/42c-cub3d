/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:48:47 by tjegades          #+#    #+#             */
/*   Updated: 2024/05/06 12:49:17 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <mlx.h>
#include <math.h>
#include <stdio.h> // printf
#include <X11/keysym.h>
#include <X11/X.h>

#define SIZE 600

typedef unsigned int t_uint;
typedef unsigned long t_ulong;

int map[6][6] = {
{1,1,1,1,1,1},
{1,0,0,1,0,1},
{1,0,0,0,0,1},
{1,1,0,0,0,1},
{1,0,0,0,1,1},
{1,1,1,1,1,1},
};

typedef struct s_mlx {
	void	*ptr;
	void	*win;
	void	*img;

	char	*img_addr;
	int		bpp;
	int		line_sz;
	int		endian;
} t_mlx;

typedef struct s_player {
	t_uint		grid_sq[2];
	t_uint		px_location[2];

	float		heading;
} t_player;

typedef struct s_params {
	t_mlx		*mlx;
	t_player	*player;
} t_params;

int	key_hook(int keycode, t_params *params);
int	close_window(t_params *params);
void draw_player(t_params p);
void draw_grid(t_params p);
int render(t_params *p);

int main(void)
{

	t_params params;
	/* -------------------------------------------------------------------------- */
	/*                                  MLX INIT                                  */
	/* -------------------------------------------------------------------------- */
	t_mlx mlx;
	params.mlx = &mlx;
	mlx.ptr = mlx_init();
	mlx.win= mlx_new_window(mlx.ptr, SIZE, SIZE, "cub3d");
	mlx.img = mlx_new_image(mlx.ptr, SIZE, SIZE);
	if (!mlx.ptr || !mlx.win || !mlx.img)
		ft_putstr_fd("Errorr initialising mlx\n", 2);

	mlx.img_addr = mlx_get_data_addr(mlx.img, &mlx.bpp, &mlx.line_sz, &mlx.endian);

	// put player on screen, curr centered in middle
	t_player player;

	params.player = &player;

	player.grid_sq[0] = 3;
	player.grid_sq[1] = 3;

	player.px_location[0] = SIZE / 2;
	player.px_location[1] = SIZE / 2;

	player.heading = 0;

	/* -------------------------------------------------------------------------- */
	/*                              MLX HOOK AND LOOP                             */
	/* -------------------------------------------------------------------------- */
	mlx_do_key_autorepeaton(mlx.ptr);
	mlx_hook(mlx.win, KeyPress, 1L, &key_hook, (void *) &params);
	mlx_hook(mlx.win, DestroyNotify, 0L, &close_window, (void *) &params);
	mlx_do_key_autorepeaton(mlx.ptr);
	mlx_loop_hook(mlx.ptr, render, &params);
	mlx_loop(mlx.ptr);
}

int render(t_params *p)
{
	draw_grid(*p);
	draw_player(*p);
	mlx_put_image_to_window(p->mlx->ptr, p->mlx->win, p->mlx->img, 0, 0);

	return (1);
}
void draw_grid(t_params p)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			// ft_printf("%i ", map[i][j]);
			if (map[i][j] == 1)
			{
				// fill square with color
				for (int row = i * 100; row < (i + 1) * 100; row++)
				{
					for (int col = j * 100; col < (j + 1) * 100; col++)
					{
						char *px = p.mlx->img_addr + row * p.mlx->line_sz + col * (p.mlx->bpp / 8);
						*(unsigned int *) px = mlx_get_color_value(p.mlx->ptr, 0x888888);
					}
				}
			}
		}
		// ft_printf("\n");
	}
	for (int row = 0; row <= SIZE; row++)
	{
		for (int col = 0; col <= SIZE; col++)
		{
			// if pixel is on a gridline
			if (row % 100 == 0 || col % 100 == 0
			|| row == SIZE - 1 || col == SIZE - 1)
			{
				char *px = p.mlx->img_addr + row * p.mlx->line_sz + col * (p.mlx->bpp / 8);
				*(unsigned int *) px = mlx_get_color_value(p.mlx->ptr, 0xffffff);
			}
		}
	}
}

void draw_player(t_params p)
{
	t_player *player = p.player;
	int pl_col = player->px_location[0] - 4;
	int pl_row = player->px_location[1] - 4;

	for (int row = pl_row; row < pl_row + 8; row++)
	{
		for (int col = pl_col; col < pl_col + 8; col++)
		{
			char *px = p.mlx->img_addr + row * p.mlx->line_sz + col * (p.mlx->bpp / 8);
			*(t_uint *) px = mlx_get_color_value(p.mlx->ptr, 0xff0000);

		}
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

int	key_hook(int keycode, t_params *params)
{
	if (keycode == XK_Escape)
		return (close_window(params));
	else if (keycode == XK_Left)
	{
		params->player->heading -= M_PI / 180;
		printf("turning left, %f\n", params->player->heading);
	}
	else if (keycode == XK_Right)
	{
		params->player->heading += M_PI / 180;
		printf("turning right, %f\n", params->player->heading);
	}
	else
		ft_printf("KEY: %i\n", keycode);
	return (1);
}