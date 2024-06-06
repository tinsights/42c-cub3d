NAME = cub3d

CFLAGS = -Wall -Werror -Wextra -O3
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

MLXDIR = mlx/
MLX =  $(MLXDIR)/libmlx.a

SRCS = main.c utils.c
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
	$(MAKE) -C $(MLXDIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
