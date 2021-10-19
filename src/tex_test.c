#include"../includes/so_long.h"
#include"../includes/get_next_line.h"
#include"../includes/libft.h"
#include "../minilibx_mac/minilibx/mlx.h"

void get_texture(t_info *info, int tex_num)
{
	int x;
	int y;

	y = 0;
	while (y < info->img.height)
	{
		x = 0;
		while (x < info->img.width)
		{
			info->tex.texture[tex_num][info->img.width * y + x] = \
			info->img.data[(info->img.line_length / 4) * y + x];
			x++;
		}
		y++;
	}
}

void texture_in(t_info *info, char *path, int tex_num)
{
	info->img.img = mlx_xpm_file_to_image(info->mlx, path, &info->img.width, &info->img.height);
	if (info->img.img == 0)
	{
		printf("Error\n");
		return ;
	}
	info->tex.tex_width[tex_num] = info->img.width;
	info->tex.tex_height[tex_num] = info->img.height;
	info->tex.texture[tex_num] = (int *)malloc(sizeof(int) * (info->img.width * info->img.height));
	info->img.data = (int *)mlx_get_data_addr(info->img.img, &(info->img.bits_per_pixel), &(info->img.line_length), &(info->img.endian));
	get_texture(info, tex_num);
}

void load_texture(t_info *info)
{
	texture_in(info, "./texture/barrel.xpm", 0);
	// texture_in(info, "./texture/barrel.xpm", 1);
	// texture_in(info, "./texture/barrel.xpm", 2);
	// texture_in(info, "./texture/barrel.xpm", 3);
	// texture_in(info, "./texture/barrel.xpm", 4);
}

void free_set(void **dest, char *src)
{
	free(*dest);
	// *dest = src;
}

int main (int argc, char **argv)
{
	t_info	info;

	info.mlx = mlx_init();
	info.mlx_win = mlx_new_window(info.mlx, 500, 500, "Hello world!");
	load_texture(&info);
	info.img.img = mlx_new_image(info.mlx, 500, 500);
	info.img.data = (int*)mlx_get_data_addr(info.img.img, &(info.img.bits_per_pixel), &(info.img.line_length), &(info.img.endian));
	// render(&info);
    int y = 0;
	int t_width = 64;
	int t_height = 64;
	int tile_size = 128;
    while(tile_size > y)
    {
        int x = 0;
		int offset_y = y * ((float)t_height / (float)tile_size);
		printf("%d\n",offset_y);
        while(tile_size > x)
        {
			int offset_x = x * ((float)t_width / (float)tile_size);
            info.img.data[y * 500 + x] = info.tex.texture[0][offset_y * 64 + offset_x];
            x++;
        }
        y++;
    }
	free_set((void **)&info.tex.texture[0], NULL);

    mlx_put_image_to_window(info.mlx, info.mlx_win, info.img.img, 0, 0);
	// system("leaks a.out");
	mlx_loop(info.mlx);
}