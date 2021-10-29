/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 23:29:24 by hiwata            #+#    #+#             */
/*   Updated: 2021/10/29 22:50:51 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/* 0 = floor 1 = wall 2 = exit 3 = object 4 = player */
bool	is_transparent(int tex_num, int color, int *j)
{
	if (tex_num == 3 || tex_num == 4)
	{
		if (color == 0x0)
		{
			*j += 1;
			return (true);
		}
	}
	return (false);
}

void	draw_map(t_info *info, int x, int y, int num)
{
	int		i;
	int		j;
	int		offset_x;
	int		offset_y;
	int		color;

	i = -1;
	while (++i < TILE_SIZE)
	{
		j = -1;
		offset_y = \
		i * ((float)info->tex.height[num] / (float)TILE_SIZE);
		while (++j < TILE_SIZE)
		{
			offset_x = \
			j * ((float)info->tex.width[num] / (float)TILE_SIZE);
			color = info->tex.texture[num] \
			[offset_y * info->tex.width[num] + offset_x];
			if (is_transparent(num, color, &j))
				continue ;
			info->img.data[(y * info->win_width) \
			+ (i * info->win_width) + j + x] = color;
		}
	}
}

void	draw_rect(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (i < info->row * TILE_SIZE)
	{
		j = 0;
		while (j < info->col * TILE_SIZE)
		{
			info->img.data[(i * info->col * TILE_SIZE) + j] = 0;
			j++;
		}
		i++;
	}
}

void	make_map(t_info *info)
{
	int		x;
	int		y;

	y = 0;
	while (y < info->row)
	{
		x = 0;
		while (x < info->col)
		{
			if (info->map[y][x] == WALL)
				draw_map(info, x * TILE_SIZE, y * TILE_SIZE, 1);
			else if (info->map[y][x] == FLOOR || info->map[y][x] == OBJECT)
				draw_map(info, x * TILE_SIZE, y * TILE_SIZE, 0);
			if (info->map[y][x] == OBJECT)
				draw_map(info, x * TILE_SIZE, y * TILE_SIZE - 7, 3);
			if (info->map[y][x] == END)
				draw_map(info, x * TILE_SIZE, y * TILE_SIZE, 2);
			x++;
		}
		y++;
	}
}

int	win_destroy(t_info *info)
{
	// free_texture(info, 4);
	mlx_destroy_window(info->mlx, info->mlx_win);
	system("leaks so_long");
	exit(0);
}

void	player_walk(t_info *info)
{
	int		new_x;
	int		new_y;

	new_x = info->px + info->p.move_x;
	new_y = info->py + info->p.move_y;
	if (info->map[new_y][new_x] != WALL)
	{
		if (info->map[new_y][new_x] != END || info->c == 0)
		{
			if (info->map[new_y][new_x] == END)
				win_destroy(info);
			info->px = new_x;
			info->py = new_y;
		}
		if (info->map[info->py][info->px] == OBJECT)
		{
			info->map[info->py][info->px] = FLOOR;
			info->c--;
		}
	}
}

void	player_move_init(t_info *info)
{
	info->p.move_x = 0;
	info->p.move_y = 0;
}

int	render(t_info *info)
{
	draw_rect(info);
	make_map(info);
	player_walk(info);
	draw_map(info, info->px * TILE_SIZE, info->py * TILE_SIZE, 4);
	player_move_init(info);
	mlx_put_image_to_window(info->mlx, info->mlx_win, info->img.img, 0, 0);
	return (1);
}

int	key_released(int keycode, t_info *info)
{
	if (keycode == ESC)
	{
		win_destroy(info);
		exit(0);
	}
	if (keycode == UP || keycode == DOWN || \
	keycode == LEFT || keycode == RIGHT)
	{
		info->move_cnt++;
		printf("Current number of moves : %d\n", info->move_cnt);
	}
	if (keycode == DOWN)
		info->p.move_y = 1;
	if (keycode == UP)
		info->p.move_y = -1;
	if (keycode == LEFT)
		info->p.move_x = -1;
	if (keycode == RIGHT)
		info->p.move_x = 1;
	return (1);
}

void free_texture(t_info *info, int tex_num)
{
	int i;

	i = tex_num + 1;
	while(i > 0)
	{
		free(info->tex.texture[tex_num]);
		info->tex.texture[tex_num] = NULL;
		i--;
		tex_num--;
	}
}

void	get_texture(t_info *info, t_data *img, int tex_num)
{
	int		x;
	int		y;

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

void	texture_in(t_info *info, t_data *img, char *path, int tex_num)
{
	img->img = \
	mlx_xpm_file_to_image(info->mlx, path, &img->width, &img->height);
	if (img->img == 0)
	{
		printf("Error\nGetting the XPM file didn't work");
		free_texture(info, tex_num);
		exit(1);
	}
	info->tex.width[tex_num] = img->width;
	info->tex.height[tex_num] = img->height;
	info->tex.texture[tex_num] = \
	(int *)malloc(sizeof(int) * (img->width * img->height));
	if (!info->tex.texture[tex_num])
	{
		free_texture(info, tex_num);
		exit(1);
	}
	img->data = (int *)mlx_get_data_addr \
	(img->img, &(img->bits_per_pixel), &(img->line_length), &(img->endian));
	get_texture(info, img, tex_num);
}

/* 0 = floor 1 = wall 2 = exit 3 = object 4 = player */
void	load_texture(t_info *info)
{
	texture_in(info, &info->img, "./texture/mossy.xpm", 0);
	texture_in(info, &info->img, "./texture/greenlight.xpm", 1);
	texture_in(info, &info->img, "./texture/pillar.xpm", 2);
	texture_in(info, &info->img, "./texture/barrel.xpm", 3);
	texture_in(info, &info->img, "./texture/eagle.xpm", 4);
}

int	main(int argc, char **argv)
{
	t_info	info;

	if (argc != 2)
		return (0);
	init_info(&info);
	info.fd = open(argv[1], O_RDONLY);
	if (info.fd < 0)
		return (0);
	read_map(&info);
	check_map(&info);
	player_move_init(&info);
	info.mlx = mlx_init();
	info.mlx_win = \
	mlx_new_window(info.mlx, info.win_width, info.win_height, "so_long");
	load_texture(&info);
	info.img.img = \
	mlx_new_image(info.mlx, info.col * TILE_SIZE, info.row * TILE_SIZE);
	info.img.data = (int *)mlx_get_data_addr(info.img.img, \
	&(info.img.bits_per_pixel), &(info.img.line_length), &(info.img.endian));
	mlx_hook(info.mlx_win, 3, 1L << 1, &key_released, &info);
	mlx_hook(info.mlx_win, 17, 1L << 17, &win_destroy, &info);
	mlx_hook(info.mlx_win, 33, 1L << 17, &win_destroy, &info);
	mlx_loop_hook(info.mlx, render, &info);
	mlx_loop(info.mlx);
}
