NAME = cub3d

CFLAGS = -g -Wall -Werror -Wextra -O3
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

MLXDIR = mlx/

MLX_URL = git@github.com:42Paris/minilibx-linux.git
MLX =  $(MLXDIR)/libmlx.a

SRCS = main.c utils.c raycast.c parse_input.c \
	parse_utils.c texture_color.c map.c map_utils.c \
	memory_cleanup.c \
	mapborder.c map_list.c \
	minimap.c minimap_utils.c \
	move.c hooks.c features.c render.c \
	raycast_utils.c dda_utils.c paint_utils.c
HDRS = cub3d.h raycast.h

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(MLX) $(OBJS) $(LIBFT) $(HDRS) Makefile
	cc $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

$(MLX):
	@if [ ! -d $(MLXDIR) ]; then \
		git clone $(MLX_URL) $(MLXDIR);\
	fi
	make -C $(MLXDIR)

$(OBJS): $(SRCS) $(HDRS)
	cc $(CFLAGS) -c $(SRCS) $(INC)

$(LIBFT):
	make -C $(LIBDIR)

clean:
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) -C $(MLXDIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	$(MAKE) -C $(MLXDIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
