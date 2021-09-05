/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 23:29:24 by hiwata            #+#    #+#             */
/*   Updated: 2021/09/03 17:22:22 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"
#include "so_long.h"
#include <mlx.h>

#define TILE_SIZE 64



void draw_map(t_info *info, int x, int y)
{
	int i;
	int j;

	i = y;
	while (i < y + TILE_SIZE)
	{
		j = x;
		while(j < x + TILE_SIZE)
		{
			info->img.data[(i * info->col * TILE_SIZE) + j] = 0XFFFFFF;
			j++;
		}
		i++;
	}

}

void windows_init(t_info *info)
{
	int i;
	int j;

	i = 0;
	while(i < info->row * TILE_SIZE)
	{
		j = 0;
		while(j < info->col * TILE_SIZE)
		{
			info->img.data[i + j] = 0;
			j++;
		}
		i++;
	}
}

void make_map(t_info *info)
{
	int x;
	int y;
// windowの初期化（多分別関数に移す）
	windows_init(info);
	y = 0;
	while (y < info->row)
	{
		x = 0;
		while (x < info->col)
		{
			if (info->map[y][x] == '1')
			{
				draw_map(info, x * TILE_SIZE, y * TILE_SIZE);
			}
			x++;
		}
		y++;
	}
}

void player_render(t_info *info, int x, int y)
{
	int i;
	int j;

	i = y + TILE_SIZE / 2;
	while (i < x + 10)
	{
		j = x + TILE_SIZE / 2;
		while (j < y + 10)
		{
			info->img.data[(i * info->col * TILE_SIZE) + j] = 0XFF0000;
			j++;
		}
		i++;
	}
}

void player_walk(t_info *info)
{
	int new_x = 0;
	int new_y = 0;

	//同時押しの時は動かない
	// if (info->p.move_x != 1 && info->p.move_y != 1)
	// {
	// 	return;
	// }
	new_x = info->px + info->p.move_x;
	new_y = info->py + info->p.move_y;
	if (info->map[new_y][new_x] != '1')
	{
		info->px = new_x;
		info->py = new_y;
		printf("newx = %d, newy = %d\n", info->px, info->py);
	}
}


int	win_destroy(t_info *info)
{
	mlx_destroy_window(info->mlx, info->mlx_win);
	exit(0);
}

void player_move_init(t_info *info)
{
	info->p.move_x = 0;
	info->p.move_y = 0;
}


int render(t_info *info)
{
	printf("aaaa\n");
	// マップのレンダー
	make_map(info);
	// プレイヤーを動かす
	player_walk(info);
	player_render(info, info->px * TILE_SIZE, info->py * TILE_SIZE);
	player_move_init(info);
	mlx_put_image_to_window(info->mlx, info->mlx_win, info->img.img, 0, 0);
	return (1);
}

int	key_pressed(int keycode, t_info *info)
{
	// printf("key=%d\n", keycode);
	if (keycode == 53)
	{
		mlx_destroy_window(info->mlx, info->mlx_win);
		exit(0);
	}
	if (keycode == 1) // S
		info->p.move_y = 1;
	if (keycode == 13) // W
		info->p.move_y = -1;
	if (keycode == 0) // A
		info->p.move_x = -1;
	if (keycode == 2) // D
		info->p.move_x = 1;
	// printf("y = %d, x = %d", info->p.move_y, info->p.move_x);
	// render(info);
	// if (keycode == 123) // <-
	// 	vars->p.turn_direction = -1;
	// if (keycode == 124) // ->
	// 	vars->p.turn_direction = 1;

	return (1);
}

int main (int argc, char **argv)
{
	t_info	info;

	if (argc != 2)
		return (0);
	// info構造体の初期化
	init_info(&info);
	info.fd = open(argv[1], O_RDONLY);
	if (info.fd < 0)
		return (0);
	// map読み込みとエラーチェック
	read_map(&info);
	check_map(&info);
	for (int i = 0; i < info.row; i++)
		printf("%s\n", info.map[i]);
	// プレイヤーオフセットのリセット
	player_move_init(&info);
	info.mlx = mlx_init();
	info.mlx_win = mlx_new_window(info.mlx, info.col * 64, info.row * 64, "Hello world!");
	info.img.img = mlx_new_image(info.mlx, info.col * 64, info.row * 64);
	info.img.data = (int*)mlx_get_data_addr(info.img.img, &(info.img.bits_per_pixel), &(info.img.line_length), &(info.img.endian));
	render(&info);
	mlx_hook(info.mlx_win, 2, 1L<<0, &key_pressed, &info);
	// mlx_hook(vars.win, 3, 1L<<1, &key_released, &vars);
	mlx_hook(info.mlx_win, 17, 1L<<17, &win_destroy, &info);
	mlx_hook(info.mlx_win, 33, 1L<<17, &win_destroy, &info);
	mlx_loop_hook(info.mlx, render, &info);
	mlx_loop(info.mlx);
	printf("px = %d, py = %d, ex = %d, ey = %d, C = %d\n", info.px, info.py, info.ex, info.ey, info.C);
	system("leaks a.out");
}