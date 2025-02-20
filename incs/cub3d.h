/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <tjegades@student.42singapore.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:10:14 by tjegades          #+#    #+#             */
/*   Updated: 2025/02/20 21:07:51 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h>
# include <math.h>
# include <mlx.h>
# include <mlx_int.h>
# include <stdbool.h>
# include <stdio.h>

# define WIN_WIDTH 1280
# define WIN_HEIGHT 800
# define MM_SQ_SZ 15
# define MM_GRID_SZ 8

# ifndef FOV
#  define FOV 100.0
# endif

# ifndef STEP
#  define STEP 0.02
# endif

# ifndef DEBUG
#  define DEBUG 0
# endif

# define INNER_TEXTURE "./incs/textures/hallway.xpm"
# define DOOR_TEXTURE "./incs/textures/tunnelv2.xpm"
# define SPRAY_TEXTURE "./incs/textures/42sg.xpm"

typedef unsigned int	t_uint;
typedef unsigned long	t_ulong;

typedef struct s_mapdata
{
	t_list				*lst;
	int					rows;
	int					rwidth;
}						t_mapdata;

typedef struct s_input
{
	char				*nxpm;
	char				*sxpm;
	char				*expm;
	char				*wxpm;
	int					fclr;
	int					cclr;
	double				heading;
	char				nswe;
	int					xpos;
	int					ypos;
	double				position[2];
	int					mwidth;
	int					mheight;
	char				**map;
}						t_input;

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
	double				heading;
	double				speed;
	double				height;
	double				vert_angle;
	bool				god;

	float				move_ws;
	float				move_ad;
	float				move_turn;
	float				move_tilt;
}						t_player;

typedef struct s_params
{
	t_mlx				mlx;
	t_input				input;
	t_player			player;
	int					curr_px[2];
	t_img				*inner;
	t_img				*north;
	t_img				*south;
	t_img				*east;
	t_img				*west;
	t_img				*spray;
	t_img				*door;
	int					fclr;
	int					cclr;
	double				fov;
	double				half_plane_width;
	int					mwidth;
	int					mheight;
	char				**map;
	bool				lights;
	bool				mouse_move;
}						t_params;

enum					e_tex_type
{
	wall,
	inner,
	spray,
	door
};

typedef struct s_ray
{
	int					id;
	int					col;
	double				dir_x;
	double				dir_y;
	double				dist_x;
	double				dist_y;
	double				delta_dist_x;
	double				delta_dist_y;

	double				projection_plane_x;
	double				perp_wall_dist;

	double				plane_x;
	double				plane_y;

	int					map_x;
	int					map_y;

	int					step_x;
	int					step_y;

	double				heading_x;
	double				heading_y;

	bool				hit;
	bool				side_x;

	int					height;
	t_img				*img;
	unsigned int		*img_data;
	struct s_ray		*next;
	enum e_tex_type		type;
}						t_ray;

typedef struct s_wall
{
	double				pos_x;
	double				pos_y;
	double				dist_to_pp;
	double				ratio;
	double				vert_shear;
	int					actual_bottom;
	int					actual_top;
	int					bottom_of_wall;
	int					top_of_wall;

	double				texture_slice;
	double				row_slice;
	int					tex_col;
	int					tex_row;
	int					true_line_height;
	float				dist;
	int					color;
	double				brightness;
}						t_wall;

typedef struct s_minimap
{
	int					sq_size;
	int					mm_size;
	int					total_size;
	double				heading;
	int					pos_y;
	int					pos_x;
	int					off_y;
	int					off_x;
	double				half_projection_plane_width;
	double				heading_x;
	double				heading_y;
	double				plane_x;
	double				plane_y;
	double				dir_x;
	double				dir_y;
	double				heading_px_row;
	double				heading_px_col;
	int					col_check;
	int					row_check;
	char				block;
	int					px_col;
	int					px_row;
}						t_minimap;

typedef struct s_move
{
	float				horiz_step;
	float				vert_step;
	float				new_height;
	float				curr_y;
	float				curr_x;
	float				heading;
	float				new_y;
	float				new_x;
}						t_move;

int						key_hook(int keycode, t_params *params);
int						close_window(t_params *params);

void					put_pixel(t_params *p, t_uint row, t_uint col,
							int colour);
void					draw_player(t_params *p);
void					draw_grid(t_params *p);
int						mouse_move(int x, int y, t_params *params);
int						mouse_click(int button, int x, int y, t_params *params);
int						key_release_hook(int keycode, t_params *params);
void					draw_crosshair(t_params *p);
int						render(t_params *p);
void					dda(t_params *params, t_ray *ray);
void					move_player(t_params *params, float direction);
void					strafe_player(t_params *params, float direction);
void					rotate_player(t_params *params, float degrees);

bool					is_wall(char c);
int						brightness_adj(int col, float brightness);
void					draw_minimap(t_params *p);

int						parse_path_color(int fd, t_input *dat);
int						read_line(int fd, char **line);

void					free_intarr(int **arr, int size);
void					free_str(char **str);
void					free_strarr(char **ptr);
void					free_maplst(t_list **lst);
void					free_xpmpath(t_input *dat);

int						nondigits(char *str);
int						validchr(char c, char *vchr);
int						validstr(char *str, char *vchr);
int						wcount(char **elem);

int						parse_map(int fd, t_input *dat);

int						remove_nl(char **line);
int						isemptyline(char *line);
int						validate_spawn(const char *str, int row, t_input *dat);

int						isvalidborder(t_mapdata *mi, t_input *dat);

int						get_tmaplist(int fd, t_mapdata *mi);

#endif
