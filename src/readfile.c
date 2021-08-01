/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 16:44:36 by hiwata            #+#    #+#             */
/*   Updated: 2021/08/01 16:09:57 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"
#include "./get_next_line.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct s_info
{
	char	**map;
	size_t	row;
	size_t	col;
	int		fd;
}				t_info;

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
}

void is_map_square(char **buf, t_info *info)
{
	int vert;
	int i;
	int j;

	vert = 0;
	while (buf[0][vert])
		vert++;
	i = 1;
	while (i < info->row)
	{
		j = 0;
		while (buf[i][j])
			j++;
		if (vert != j)
		{
			printf("Not square map\n");
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
			printf("Invalid Map\n");
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

void	check_map(t_info *info)
{
	is_map_square(info->map, info);
}

int main (int argc, char **argv)
{
	t_info	info;

	// if (argc != 2)
		// return (0);
	init_info(&info);
	info.fd = open(argv[1], O_RDONLY);
	if (info.fd < 0)
		return (0);
	read_map(&info);
	check_map(&info);
	for (int i = 0; i < info.row; i++)
	{
		printf("%s\n", info.map[i]);
	}
	system("leaks a.out");
}