NAME = cub3d

CFLAGS = -g -Wall -Werror -Wextra -O3
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes -Iincs

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

MLXDIR = mlx/

MLX_URL = git@github.com:42Paris/minilibx-linux.git
MLX =  $(MLXDIR)/libmlx.a

SRCDIR = srcs/
SRCS = $(addprefix $(SRCDIR), \
    main.c utils.c raycast.c parse_input.c \
    parse_utils.c texture_color.c map.c map_utils.c \
    memory_cleanup.c \
    mapborder.c map_list.c \
    minimap.c minimap_utils.c \
    move.c hooks.c features.c render.c \
    raycast_utils.c dda_utils.c paint_utils.c)

HDRS = $(addprefix ./incs/, cub3d.h raycast.h)

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(MLX) $(OBJS) $(LIBFT) $(HDRS) Makefile
	cc $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

$(MLX):
	@if [ ! -d $(MLXDIR) ]; then \
		git clone $(MLX_URL) $(MLXDIR);\
	fi
	make -C $(MLXDIR)

$(SRCDIR)%.o: $(SRCDIR)%.c $(HDRS)
	cc $(CFLAGS) -c $< $(INC) -o $@

$(LIBFT):
	make -C $(LIBDIR)

clean:
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) -C $(MLXDIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re