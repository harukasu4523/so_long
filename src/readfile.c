/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 16:44:36 by hiwata            #+#    #+#             */
/*   Updated: 2021/10/31 22:17:29 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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

void	init_info(t_info *info)
{
	info->map = NULL;
	info->row = 0;
	info->col = 0;
	info->c = 0;
	info->tc = 0;
	info->px = 0;
	info->py = 0;
	info->ex = 0;
	info->ey = 0;
	info->move_cnt = 0;
}

void	is_map_square(char **buf, t_info *info)
{
	int	vert;
	int	i;
	int	j;

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

bool	set_map(char *line, t_info *info)
{
	char	**old;
	char	**new;
	int		i;

	old = info->map;
	new = malloc(sizeof(char *) * (info->row + 1));
	if (!new)
		return (false);
	i = 0;
	while (i < info->row)
	{
		new[i] = ft_strdup(old[i]);
		if (!new[i])
			return (freei_return(new, i, false));
		i++;
	}
	new[i] = ft_strdup(line);
	if (!new[i])
		return (freei_return(new, i, false));
	info->map = new;
	info->row++;
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

int	find_chr(const char *s1, const char *s2)
{
	int	i;
	int	j;
	int	checker;

	if (! *s1)
		return (0);
	i = 0;
	while (s1[i])
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

void	read_map(t_info *info)
{
	int		ret;
	char	*line;

	ret = get_next_line(info->fd, &line);
	while (ret > 0)
	{
		if (!(find_chr(line, "10CPE")))
		{
			printf("Error\nInvalid Map\n");
			exit (1);
		}
		set_map(line, info);
		free(line);
		ret = get_next_line(info->fd, &line);
	}
	free_line(info, ret, line);
	if (ret == -1)
		exit (1);
	close(info->fd);
}

void	get_location_player(t_info *info, int x, int y)
{
	if (info->px == 0 && info->py == 0)
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

void	get_location_vent(t_info *info, int x, int y)
{
	if (info->ex == 0 && info->ey == 0)
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

void	get_location_object(t_info *info, char**map, int y, int x)
{
	if (map[y][x] == 'C')
		info->c++;
	else if (map[y][x] == 'P')
		get_location_player(info, x, y);
	else if (map[y][x] == 'E')
		get_location_vent(info, x, y);
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
	char	**map;

	map = info->map;
	if ((x == 0 || y == 0 || x == info->col \
	|| y == info->row) && map[y][x] != WALL)
		return (false);
	if (map[y][x] == WALL || map[y][x] == FLOOR \
	|| map[y][x] == OBJECT || map[y][x] == END)
		return (true);
	if (map[y][x] == 'C')
	{
		info->tc += 1;
		map[y][x] = OBJECT;
	}
	else if (map[y][x] == 'E')
	{
		map[y][x] = END;
	}
	else
		map[y][x] = FLOOR;
	return (is_map_surrounded(info, x + 1, y) && \
	is_map_surrounded(info, x - 1, y) && is_map_surrounded(info, x, y + 1) \
	&& is_map_surrounded(info, x, y - 1));
}

void	in_location_c(t_info *info, int *bufx, int *bufy)
{
	int	i;
	int	j;
	int	k;

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
				k++;
			}
			j++;
		}
		i++;
	}
}

void	get_location_c(t_info *info)
{
	int	*bufx;
	int	*bufy;

	bufx = malloc(sizeof(int) * info->c);
	if (!bufx)
	{
		printf("Error\nFailed to allocate malloc\n");
		exit(1);
	}
	bufy = malloc(sizeof(int) * info->c);
	if (!bufy)
	{
		printf("Error\nFailed to allocate malloc\n");
		exit(1);
	}
	in_location_c(info, bufx, bufy);
	info->cx = bufx;
	info->cy = bufy;
	free(bufx);
	free(bufy);
}

void	check_map(t_info *info)
{
	is_map_square(info->map, info);
	search_location_object(info->map, info);
	if (!is_map_surrounded(info, info->px, info->py))
	{
		printf("Error\nThis map is not surrounded\n");
		exit(1);
	}
	if (info->map[0][0] != '1' || info->map[0][info->col - 1] != '1' \
	|| info->map[info->row - 1][0] != '1' \
	|| info->map[info->row - 1][info->col - 1] != '1')
	{
		printf("EError\nThis map is not surrounded\n");
		exit(1);
	}
	if (info->c != info->tc)
	{
		printf("Error\nIt is out of reach of C");
		exit(1);
	}
	get_location_c(info);
	info->win_width = info->col * TILE_SIZE;
	info->win_height = info->row * TILE_SIZE;
}
