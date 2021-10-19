/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 23:29:24 by hiwata            #+#    #+#             */
/*   Updated: 2021/10/18 20:51:07 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"
#include "so_long.h"
#include <mlx.h>

#define TILE_SIZE 30

void draw_map(t_info *info, int x, int y, int tex_num)
{
	int i;
	int j;
	int offset_x;
	int offset_y;

	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		//表示画像の最適化
		offset_y = i * ((float)info->tex.tex_height[tex_num] / (float)TILE_SIZE);
		while(j < TILE_SIZE)
		{
			offset_x = j * ((float)info->tex.tex_width[tex_num] / (float)TILE_SIZE);
			info->img.data[(y * info->col * TILE_SIZE) + (i * info->col * TILE_SIZE) + j + x] = info->tex.texture[tex_num][offset_y * info->tex.tex_width[tex_num] + offset_x];
			j++;
		}
		i++;
	}

}

void draw_rect(t_info *info)
{
	int i;
	int j;

	i = 0;
	while(i < info->row * TILE_SIZE)
	{
		j = 0;
		while(j < info->col * TILE_SIZE)
		{
			info->img.data[(i * info->col * TILE_SIZE) + j] = 0;
			j++;
		}
		i++;
	}
}

void player_render(t_info *info, int x, int y, int tex_num)
{
	int i;
	int j;
	int offset_x;
	int offset_y;
	int color;

	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		//表示画像の最適化
		offset_y = i * ((float)info->tex.tex_height[tex_num] / (float)TILE_SIZE);
		while(j < TILE_SIZE)
		{
			offset_x = j * ((float)info->tex.tex_width[tex_num] / (float)TILE_SIZE);
			color = info->tex.texture[tex_num][offset_y * info->tex.tex_width[tex_num] + offset_x];
			if (color == 0x0)
			{
				j++;
				continue;
			}
			info->img.data[(y * info->col * TILE_SIZE) + (i * info->col * TILE_SIZE) + j + x] = color;
			j++;
		}
		i++;
	}
}

void make_map(t_info *info)
{
	int x;
	int y;

	y = 0;
	while (y < info->row)
	{
		x = 0;
		while (x < info->col)
		{
			//壁
			if (info->map[y][x] == '1')
			{
				draw_map(info, x * TILE_SIZE, y * TILE_SIZE, 1);
			}
			//動けるゾーン
			else if(info->map[y][x] == '*' || info->map[y][x] == 'G')
			{
				draw_map(info, x * TILE_SIZE, y * TILE_SIZE, 0);
			}
			//取得物
			if (info->map[y][x] == 'G')
			{
				player_render(info, x * TILE_SIZE, y * TILE_SIZE, 3);
			}
			//出口
			if (info->map[y][x] == 'e')
			{
				draw_map(info, x * TILE_SIZE, y * TILE_SIZE, 2);
			}

			x++;
		}
		y++;
	}
}

int	win_destroy(t_info *info)
{
	mlx_destroy_window(info->mlx, info->mlx_win);
	system("leaks so_long");
	exit(0);
}

void player_walk(t_info *info)
{
	int new_x;
	int new_y;

	//同時押しの時は動かない
	// if (info->p.move_x != 1 && info->p.move_y != 1)
	// {
	// 	return;
	// }
	new_x = info->px + info->p.move_x;
	new_y = info->py + info->p.move_y;
	if (info->map[new_y][new_x] != '1' )
	{
		// Cの数が0になったらeに入れるようにする
		if (info->map[new_y][new_x] != 'e' || info->C == 0)
		{
			if (info->map[new_y][new_x] == 'e')
				win_destroy(info);
			info->px = new_x;
			info->py = new_y;
		}
		// 取得物と重なったらGをフロアと同じに変え取得物の総数を減らしていく
		if (info->map[info->py][info->px] == 'G')
		{
			info->map[info->py][info->px] = '*';
			info->C--;
		}
	// printf("newx = %d, newy = %d\n", info->px, info->py);
	}
}



void player_move_init(t_info *info)
{
	info->p.move_x = 0;
	info->p.move_y = 0;
}


int render(t_info *info)
{
	// printf("aaaa\n");
	// windowの初期化
	draw_rect(info);
	// マップのレンダー
	make_map(info);
	// px,pyを入力方向に動かす
	player_walk(info);
	//動いた場所を描く
	player_render(info, info->px * TILE_SIZE, info->py * TILE_SIZE, 4);
	// オフセットの初期化
	player_move_init(info);
	// windowへの反映
	mlx_put_image_to_window(info->mlx, info->mlx_win, info->img.img, 0, 0);
	return (1);
}

int	key_released(int keycode, t_info *info)
{
	// printf("key=%d\n", keycode);
	if (keycode == 53)
	{
		mlx_destroy_window(info->mlx, info->mlx_win);
		exit(0);
	}
	if (keycode == 1) // S
	{
		info->p.move_y = 1;
		info->move_cnt++;
		printf("Current number of moves : %d\n",info->move_cnt);
	}
	if (keycode == 13) // W
	{
		info->p.move_y = -1;
		info->move_cnt++;
		printf("Current number of moves : %d\n",info->move_cnt);
	}
	if (keycode == 0) // A
	{
		info->p.move_x = -1;
		info->move_cnt++;
		printf("Current number of moves : %d\n",info->move_cnt);
	}
	if (keycode == 2) // D
	{
		info->p.move_x = 1;
		info->move_cnt++;
		printf("Current number of moves : %d\n",info->move_cnt);
	}
	// info->move_cnt++;
	// printf("Current number of moves : %d\n",info->move_cnt);
	// printf("y = %d, x = %d", info->p.move_y, info->p.move_x);
	// render(info);
	// if (keycode == 123) // <-
	// 	vars->p.turn_direction = -1;
	// if (keycode == 124) // ->
	// 	vars->p.turn_direction = 1;

	return (1);
}

void get_texture(t_info *info, t_data *img, int tex_num)
{
	int x;
	int y;

	printf("num = %d\n",tex_num);
	y = 0;
	while (y < info->img.height)
	{
		x = 0;
		while (x < info->img.width)
		{
			info->tex.texture[tex_num][img->width * y + x] = \
			img->data[(img->line_length / 4) * y + x];
			x++;
		}
		y++;
	}
}

void texture_in(t_info *info, t_data *img, char *path, int tex_num)
{
	//xpmファイルをvoid*の配列に変える
	img->img = mlx_xpm_file_to_image(info->mlx, path, &img->width, &img->height);
	//エラー処理
	if (img->img == 0)
	{
		printf("Error\n");
		return ;
	}
	//tex構造体に取得したtextureの縦横を入れる
	info->tex.tex_width[tex_num] = img->width;
	info->tex.tex_height[tex_num] = img->height;
	//取得した画像分の配列を確保
	info->tex.texture[tex_num] = (int *)malloc(sizeof(int) * (img->width * img->height));
	//imgデータをint*の配列に変える。
	img->data = (int *)mlx_get_data_addr(img->img, &(img->bits_per_pixel), &(img->line_length), &(img->endian));
	//dataを支える形にしてtex構造体に入れ込む
	get_texture(info, img, tex_num);
}

void load_texture(t_info *info)
{
	//0 = 床 1 = 壁 2 = 出口 3 = 取得物4 = player
	texture_in(info, &info->img, "./texture/mossy.xpm", 0);
	texture_in(info, &info->img, "./texture/greenlight.xpm", 1);
	texture_in(info, &info->img, "./texture/pillar.xpm", 2);
	texture_in(info, &info->img, "./texture/barrel.xpm", 3);
	texture_in(info, &info->img, "./texture/eagle.xpm", 4);
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
	info.move_cnt = 0;
	// map読み込みとエラーチェック
	read_map(&info);
	check_map(&info);
	for (int i = 0; i < info.row; i++)
		printf("%s\n", info.map[i]);
	// プレイヤーオフセットのリセット
	player_move_init(&info);
	info.mlx = mlx_init();
	info.mlx_win = mlx_new_window(info.mlx, info.col * TILE_SIZE, info.row * TILE_SIZE, "Hello world!");
	//textureの読み込み
	load_texture(&info);
	info.img.img = mlx_new_image(info.mlx, info.col * TILE_SIZE, info.row * TILE_SIZE);
	info.img.data = (int*)mlx_get_data_addr(info.img.img, &(info.img.bits_per_pixel), &(info.img.line_length), &(info.img.endian));
	// render(&info);
	mlx_hook(info.mlx_win, 3, 1L<<1, &key_released, &info);
	// mlx_hook(vars.win, 3, 1L<<1, &key_released, &vars);
	mlx_hook(info.mlx_win, 17, 1L<<17, &win_destroy, &info);
	mlx_hook(info.mlx_win, 33, 1L<<17, &win_destroy, &info);
	mlx_loop_hook(info.mlx, render, &info);
	mlx_loop(info.mlx);
	printf("px = %d, py = %d, ex = %d, ey = %d, C = %d\n", info.px, info.py, info.ex, info.ey, info.C);
}