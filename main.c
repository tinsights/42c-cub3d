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

int map[5][6] = {
{1,1,1,1,1,1},
{1,0,0,1,0,1},
{1,0,1,0,0,1},
{1,1,0,0,0,1},
{1,1,1,1,1,1},
};

typedef struct s_mlx {
	void *ptr;
	void *win;
	void *img;
} t_mlx;

int	key_hook(int keycode, t_mlx *mlx);

int main(void)
{
	for (int i = 0; i < 5; i ++)
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
	mlx.win= mlx_new_window(mlx.ptr, 600, 600, "cub3d");
	if (!mlx.ptr || !mlx.win)
		ft_putstr_fd("Errorr initialising mlx\n", 2);

	mlx_key_hook(mlx.win, &key_hook, (void *) &mlx);
	mlx_loop(mlx.ptr);
}


int	close_window(t_mlx *mlx)
{
	// mlx_destroy_image(mlx->ptr, mlx->img);
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