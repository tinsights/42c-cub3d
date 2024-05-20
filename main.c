/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:48:47 by tjegades          #+#    #+#             */
/*   Updated: 2024/05/06 12:49:17 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <mlx.h>

#include <X11/keysym.h>
#include <X11/X.h>

#define SIZE 600

int map[6][6] = {
{1,1,1,1,1,1},
{1,0,0,1,0,1},
{1,0,1,0,0,1},
{1,1,0,0,0,1},
{1,0,0,0,1,1},
{1,1,1,1,1,1},
};

typedef struct s_mlx {
	void	*ptr;
	void	*win;
	void	*img;

	char	*img_addr;
	int		bpp;
	int		line_sz;
	int		endian;
} t_mlx;

int	key_hook(int keycode, t_mlx *mlx);
int	close_window(t_mlx *mlx);

int main(void)
{
	for (int i = 0; i < 6; i ++)
	{
		for (int j = 0; j < 6; j++)
			ft_printf("%i ", map[i][j]);
		ft_printf("\n");
	}

	/* -------------------------------------------------------------------------- */
	/*                                  MLX INIT                                  */
	/* -------------------------------------------------------------------------- */
	t_mlx mlx;
	mlx.ptr = mlx_init();
	mlx.win= mlx_new_window(mlx.ptr, SIZE, SIZE, "cub3d");
	mlx.img = mlx_new_image(mlx.ptr, SIZE, SIZE);
	if (!mlx.ptr || !mlx.win || !mlx.img)
		ft_putstr_fd("Errorr initialising mlx\n", 2);

	mlx.img_addr = mlx_get_data_addr(mlx.img, &mlx.bpp, &mlx.line_sz, &mlx.endian);

	for (int row = 0; row <= SIZE; row++)
	{
		for (int col = 0; col <= SIZE; col++)
		{
			// if pixel is on a gridline
			if (row % 100 == 0 || col % 100 == 0
			|| row == SIZE - 1 || col == SIZE - 1)
			{
				char *px = mlx.img_addr + row * mlx.line_sz + col * (mlx.bpp / 8);
				*(unsigned int *) px = mlx_get_color_value(mlx.ptr, 0xffffff);
			}
		}
	}
	mlx_put_image_to_window(mlx.ptr, mlx.win, mlx.img, 0, 0);
	/* -------------------------------------------------------------------------- */
	/*                              MLX HOOK AND LOOP                             */
	/* -------------------------------------------------------------------------- */
	mlx_key_hook(mlx.win, &key_hook, (void *) &mlx);
	mlx_hook(mlx.win, DestroyNotify, 0L, &close_window, (void *) &mlx);
	mlx_loop(mlx.ptr);
}


int	close_window(t_mlx *mlx)
{
	mlx_destroy_image(mlx->ptr, mlx->img);
	mlx_destroy_window(mlx->ptr, mlx->win);
	mlx_destroy_display(mlx->ptr);
	free(mlx->ptr);
	exit(1);
	return (1);
}

int	key_hook(int keycode, t_mlx *mlx)
{
	if (keycode == XK_Escape)
		return (close_window(mlx));
	else
		ft_printf("KEY: %i\n", keycode);
	return (1);
}