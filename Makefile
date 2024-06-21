NAME = cub3d

CFLAGS = -g -Wall -Werror -Wextra -O3
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

MLXDIR = mlx/
MLX =  $(MLXDIR)/libmlx.a

SRCS = main.c utils.c raycast.c parse_input.c \
	parse_utils.c scene_info.c map.c map_utils.c\
	memory_cleanup.c get_next_line/get_next_line.c \
	mapborder.c print.c\
		get_next_line/get_next_line_utils.c
HDRS = cub3d.h ./get_next_line/get_next_line.h

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX) $(HDRS) Makefile
	cc $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

#$(OBJS): $(SRCS) $(HDRS)
#	cc $(CFLAGS) -c $(SRCS) $(INC)

%.o: %.c $(HDRS)
	cc $(CFLAGS) -c $< $(INC) -o $@
	
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
