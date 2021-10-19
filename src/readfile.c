/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 16:44:36 by hiwata            #+#    #+#             */
/*   Updated: 2021/09/07 22:16:33 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"
#include "so_long.h"


// ファイルを読み込みエラーのチェクと、それぞれのオブジェクトの座標を取る

bool	freei_return(char **ptr, int n, bool ret)
{
	int	i;

	if (!ptr)
		return (ret);
	i = 0;
	while (i < n)
		free(ptr[i++]);
	free(ptr);
	return (ret);
}

void init_info(t_info *info)
{
	info->map = NULL;
	info->row = 0;
	info->col = 0;
	info->C = 0;
	info->tC = 0;
	info->px = 0;
	info->py = 0;
	info->ex = 0;
	info->ey = 0;
}

void is_map_square(char **buf, t_info *info)
{
	int vert;
	int i;
	int j;

	vert = 0;
	while (buf[0][vert])
		vert++;
	info->col = vert;
	i = 1;
	while (i < info->row)
	{
		j = 0;
		while (buf[i][j])
			j++;
		if (vert != j)
		{
			printf("Error\nNot square map\n");
			exit (1);
		}
		i++;
	}
}

//一度info->mapに情報を入れてからエラーか判定する

bool set_map(char *line, t_info *info)
{
	char	**old;
	char	**new;
	int		i;

	old = info->map;
	new = malloc(sizeof(char *) * (info->row + 1));
	if (!new)
		return (false);
	i = 0;
	//列の数だけ昔格納したmapをnewに移していく
	while (i < info->row)
	{
		new[i] = ft_strdup(old[i]);
		if (!new[i])
			return (freei_return(new, i, false));
		i++;
	}
	//今回きた行をnewに追加してそれをinfo->mapに格納
	new[i] = ft_strdup(line);
	if(!new[i])
		return (freei_return(new, i, false));
	info->map = new;
	info->row++;
	//oldに溜まったマロックをfreeして返す
	return (freei_return(old, info->row - 1, true));
}

void	free_line(t_info *info, int ret, char *line)
{
	if (ret == 0)
	{
		set_map(line, info);
		free(line);
	}
}

int	find_chr(const char *s1,const char *s2)
{
	int	i;
	int	j;
	int checker;

	if (!*s1)
		return(0);
	i = 0;
	while(s1[i])
	{
		j = 0;
		checker = 0;
		if (s1[i] != s2[j])
		{
			while (s2[j])
			{
				if (s1[i] == s2[j])
					checker = 1;
				j++;
			}
			if (!checker)
				return (0);
		}
		i++;
	}
	return (1);
}

//mapへの詰め込みはcub3dのset_map参照
//gnlは最終行を読み込んだら”0”を返すので最後の読み込みに注意する
//row = 列数(縦の配列) col = 行数(横の配列)

void	read_map(t_info *info)
{
	int		ret;
	char	*line;
	int i;

	i = 0;
	//マップの読み込みと受け取り
	ret = get_next_line(info->fd, &line);
	while (ret > 0)
	{
		// printf("-------%d------\n",i++);
		//いらない要素があればエラーだす
		if(!(find_chr(line, "10CPE")))
		{
			printf("Error\nInvalid Map\n");
			exit (1);
		}
		//info->mapへの詰め込み
		set_map(line, info);
		free(line);
		ret = get_next_line(info->fd, &line);
	}
	free_line(info, ret, line);
	if (ret == -1)
		exit (1);
	close(info->fd);
}

void	get_location_object(t_info *info, char**map, int y, int x)
{
	if (map[y][x] == 'C')
	{
		info->C++;
	}
	else if (map[y][x] == 'P')
	{
		if(info->px == 0 && info->py == 0)
		{
			info->px = x;
			info->py = y;
		}
		else
		{
			printf("Error\nThere are multiple players on the map\n");
			exit(1);
		}
	}
	else if (map[y][x] == 'E')
	{
		if(info->ex == 0 && info->ey == 0)
		{
			info->ex = x;
			info->ey = y;
		}
		else
		{
			printf("Error\nThere are multiple end points on the map\n");
			exit(1);
		}
	}
}

void	search_location_object(char **map, t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (i < info->row)
	{
		j = 0;
		while (j < info->col)
		{
			if (map[i][j] != '1' && map[i][j] != '0')
				get_location_object(info, map, i, j);
			j++;
		}
		i++;
	}
	if (info->px == 0 && info->py == 0)
	{
		printf("Error\nThere are no player on the map\n");
		exit (1);
	}
	if (info->ex == 0 && info->ey == 0)
	{
		printf("Error\nThere are no end point on the map\n");
		exit (1);
	}
}

bool	is_map_surrounded(t_info *info, int x, int y)
{
	char **map = info->map;

	//maxとminの場所が決まっているのでそれ以上探索しないようにする
	if ((x == 0 || y == 0 || x == info->col || y == info->row) && map[y][x] != '1')
		return (false);
	if (map[y][x] == '1' || map[y][x] == '*' || map[y][x] == 'G' || map[y][x] == 'e')
		return (true);
	if (map[y][x] == 'C')
	{
		info->tC += 1;
		//座標をとっていないので別のものに置き換える
		map[y][x] = 'G';
	}
	// 置き換えいらないかも
	else if (map[y][x] == 'E')
	{
		map[y][x] = 'e';
	}
	else
		map[y][x] = '*';
	return (is_map_surrounded(info, x + 1, y) && is_map_surrounded(info, x - 1, y) && \
	is_map_surrounded(info, x, y + 1) && is_map_surrounded(info, x, y - 1));
}

void get_location_C(t_info *info)
{
	int *bufx;
	int *bufy;
	int i;
	int j;
	int k;

	bufx = malloc(sizeof(int) * info->C);
	if (!bufx)
	{
		printf("Error\nFailed to allocate malloc\n");
		exit(1);
	}
	bufy = malloc(sizeof(int) * info->C);
	if (!bufy)
	{
		printf("Error\nFailed to allocate malloc\n");
		exit(1);
	}
	i = 0;
	k = 0;
	while (i < info->row)
	{
		j = 0;
		while (j < info->col)
		{
			if (info->map[i][j] == 'G')
			{
				bufx[k] = j;
				bufy[k] = i;
				printf("%d : (x, y) = (%d, %d)\n", k, j , i);
				k++;
			}
			j++;
		}
		i++;
	}
	info->cx = bufx;
	info->cy = bufy;
	// free(bufx);
	// free(bufy);
	i = 0;
	while(i < info->C)
	{
		printf("%d : (x, y) = (%d, %d)\n", i, info->cx[i], info->cy[i]);
		i++;
	}
}



void	check_map(t_info *info)
{
	//mapが長方形であるか調べる
	is_map_square(info->map, info);
	//CPEが規定個数分あるか調べる
	search_location_object(info->map, info);
	//1で囲まれているか、Cが行けるところに配置されているかを調べる
	if (!is_map_surrounded(info, info->px, info->py))
	{
		printf("Error\nThis map is not surrounded\n");
		exit(1);
	}
	// 四角が1かどうか調べる
	if (info->map[0][0] != '1' || info->map[0][info->col - 1] != '1' || info->map[info->row - 1][0] != '1' || info->map[info->row - 1][info->col - 1] != '1')
	{
		printf("EError\nThis map is not surrounded\n");
		exit(1);
	}
	//行ける場所にあるCと全てのCの数が同じか調べる
	if (info->C != info->tC)
	{
		printf("Error\nIt is out of reach of C");
		exit(1);
	}
	//Cの座標を取る
	get_location_C(info);
}

// int main (int argc, char **argv)
// {
// 	t_info	info;

// 	// if (argc != 2)
// 		// return (0);
// 	init_info(&info);
// 	info.fd = open(argv[1], O_RDONLY);
// 	if (info.fd < 0)
// 		return (0);
// 	read_map(&info);
// 	check_map(&info);
// 	for (int i = 0; i < info.row; i++)
// 	{
// 		printf("%s\n", info.map[i]);
// 	}
// 	printf("px = %d, py = %d, ex = %d, ey = %d, C = %d\n", info.px, info.py, info.ex, info.ey, info.C);
// 	system("leaks a.out");
// }