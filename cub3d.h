/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:10:14 by tjegades          #+#    #+#             */
/*   Updated: 2024/05/21 13:10:15 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include "libft.h"
#include <mlx.h>
#include <math.h>
#include <stdio.h> // printf
#include <X11/keysym.h>
#include <X11/X.h>

#define SIZE 600

typedef unsigned int t_uint;
typedef unsigned long t_ulong;

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

	double		heading;
} t_player;

typedef struct s_params {
	t_mlx		*mlx;
	t_player	*player;
} t_params;

int	key_hook(int keycode, t_params *params);
int	close_window(t_params *params);

void put_pixel(t_params p, t_uint row, t_uint col, int colour);
void draw_player(t_params p);
void draw_grid(t_params p);
int render(t_params *p);


extern int map[6][6];

#endif