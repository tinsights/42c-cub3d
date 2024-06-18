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
# include <stdbool.h>
# include <stdio.h> // printf
# include <fcntl.h>

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
# define ONEORZERO 1 //1 or 0

extern int				map[MHEIGHT][MWIDTH];

typedef unsigned int	t_uint;
typedef unsigned long	t_ulong;

typedef struct s_mapinfo
{
	t_list	*lst;
	int	rows; //count rows
	int	cols; //count cols
	int	rwidth; //current row width //replace it with cols
	int	nswe[4][3];
	int	irow;
	int	icol;
	//int	max; //maxwidth
	//int	pflag;//to check duplicate player
} t_mapinfo;

typedef struct s_input
{
	char	*nxpm;
	char	*sxpm;
	char	*expm;
	char	*wxpm;
	int	fcolor[3];
	int	ccolor[3];
	int	xdir;//xdir based on NSWE
	int	ydir;//ydir based on NSWE
	int	xpos;//xcoordinate of player
	int	ypos;//ycoordinate of player
	//int 	**map;
	char	**map;
} t_input;

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
	double				speed;
	double				height;
	double				vert_angle;
}						t_player;

typedef struct s_params
{
	t_mlx				*mlx;
	t_player			*player;

	int					clicked_px[2];

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
void					draw_crosshair(t_params p);
int						render(t_params *p);

//parse_input.c
void 	get_data(char *mapfile, t_input	*dat);

//scene_info.c
int	validate_typeid(int fd, t_input *dat);
int	read_line(int fd, char **line);

//memory_cleanup.c
void	free_intarr(int **arr, int size);
void	free_str(char **str);
void	free_strarr(char **ptr);
int	free_maplst(t_list **lst);
int	free_return1(t_list **lst, char **str);

//parse_utils.c
int	nondigits(char *str);
int	validchar(char c, char *vchr);
int	validstr(char *str, char *vchr);
int	wcount(char **elem);

//map.c
int	parse_map(int fd, t_input *dat);

//map_utils.c
int	remove_nl(char **line);
int	isemptyline(char *line);
int	validate_nswe(const char *str, t_mapinfo *mi, t_input *dat);

//mapborder.c
int	isvalidborder(t_mapinfo *mi, t_input *dat);

//print.c
void	print_mi(t_mapinfo *mi);
void	print_lst(t_list *lst);
void	print_input(t_input *dat);
void	print_dblarr(char **arr);

#endif
