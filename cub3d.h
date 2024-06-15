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
# define CUB3D_H

# include "libft.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <mlx.h>
# include <mlx_int.h>
# include <stdbool.h>
# include <stdio.h> // printf

# define WIN_WIDTH 1280
# define WIN_HEIGHT 800

# ifndef FOV
#  define FOV 100.0
# endif

# ifndef DEBUG
#  define DEBUG false
# endif

/** for developing
 * to be replaced by input validation
 */
# define MHEIGHT 8
# define MWIDTH 24

extern int				map[MHEIGHT][MWIDTH];

typedef unsigned int	t_uint;
typedef unsigned long	t_ulong;

typedef struct s_mlx
{
	void				*ptr;
	void				*win;
	void				*img;

	char				*img_addr;
	int					bpp;
	int					line_sz;
	int					endian;
}						t_mlx;

typedef struct s_player
{
	double				position[2];

	double				heading; // in rads
	double				heading_x;
	double				heading_y;
	double				speed;
	double				height;
	double				vert_angle;
}						t_player;

typedef struct s_params
{
	t_mlx				*mlx;
	t_player			*player;

	int					clicked_px[2];

	t_img				*inner;
	t_img				*north;
	t_img				*south;
	t_img				*east;
	t_img				*west;
	double				fov;
}						t_params;

int						key_hook(int keycode, t_params *params);
int						close_window(t_params *params);

void					put_pixel(t_params p, t_uint row, t_uint col,
							int colour);
void					draw_player(t_params p);
void					draw_grid(t_params p);
int						mouse_move(int x, int y, t_params *params);
int						mouse_click(int button, int x, int y, t_params *params);
int						key_release_hook(int keycode, t_params *params);
void					draw_crosshair(t_params p);
int						render(t_params *p);

#endif