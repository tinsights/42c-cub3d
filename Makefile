NAME = cub3d

CFLAGS = -g -Wall -Werror -Wextra -O3
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

MLXDIR = mlx/
MLX =  $(MLXDIR)/libmlx.a

SRCS = main.c utils.c raycast.c parse_input.c \
	parse_utils.c texture_color.c map.c map_utils.c\
	memory_cleanup.c \
	mapborder.c print.c \
	minimap.c 
HDRS = cub3d.h

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX) $(HDRS) Makefile
	cc $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

$(OBJS): $(SRCS) $(HDRS)
	cc $(CFLAGS) -c $(SRCS) $(INC)
	
$(MLX):
	make -C $(MLXDIR)

$(LIBFT):
	make -C $(LIBDIR)

clean:
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) -C $(MLXDIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	#$(MAKE) -C $(MLXDIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
