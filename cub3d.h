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
# define MWIDTH 11
# define ONEORZERO 48 //49 or 48

//extern char	map[MHEIGHT][MWIDTH];


typedef unsigned int	t_uint;
typedef unsigned long	t_ulong;

typedef struct s_mapinfo
{
	t_list	*lst;
	int	rows; //row size
	int	rwidth; //same as col size
	double	nswe[4][4];
	int	irow;
	int	icol;
} t_mapinfo;

typedef struct s_input
{
	char	*nxpm;
	char	*sxpm;
	char	*expm;
	char	*wxpm;
	int	fcolor[3];
	int	ccolor[3];
	int	fclr;
	int	cclr;
	double	heading;
	//int	xdir;//xdir based on NSWE -->remove
	//int	ydir;//ydir based on NSWE -->remove
	char	nswe;//set to N S W or E
	int	xpos;//xcoordinate of player
	int	ypos;//ycoordinate of player
	int	mwidth;
	int	mheight;
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
	bool				god;

	float					move_ws;
	float					move_ad;
	float					move_turn;
	float					move_tilt;
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
	t_img				*spray;
	t_img				*door;
	int				fclr;//floor
	int				cclr;//ceiling
	double				fov;
	double				half_plane_width; // to be used
	int				mwidth;
	int				mheight;
	char				**map;
}						t_params;

typedef struct s_ray
{
	int id;
	int col; //pixel col;
	double dir_x;
	double dir_y;
	double dist_x;
	double dist_y;
	double delta_dist_x;
	double delta_dist_y;

	double projection_plane_x;
	double perp_wall_dist;
	
	double plane_x;
	double plane_y;

	int map_x;
	int map_y;

	int step_x;
	int step_y;

	double				heading_x;
	double				heading_y;

	bool hit;
	bool side_x;

	int height;
	t_img *img;
	unsigned int	*img_data;
	struct s_ray	*next;
} t_ray;

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
void 					dda(t_params *params, t_ray *ray);
void move_player(t_params *params, float direction);
void strafe_player(t_params *params, float direction);
void rotate_player(t_params *params, float degrees);

bool is_wall(char c);


//parse_input.c
void 	get_data(char *mapfile, t_input	*dat);

//scene_info.c
int	validate_typeid(int fd, t_input *dat);
int	read_line(int fd, char **line);

//memory_cleanup.c
void	free_intarr(int **arr, int size);
void	free_str(char **str);
void	free_strarr(char **ptr);
char	*free_strarr2(char **ptr, int len);
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
void	print_intarr(int **arr, int rows, int cols);

#endif
