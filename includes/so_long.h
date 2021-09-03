/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 23:31:44 by hiwata            #+#    #+#             */
/*   Updated: 2021/09/03 16:17:26 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

#include <stdio.h>
#include <stdbool.h>

typedef struct s_player
{
	int move_y;
	int move_x;
}				t_player;

typedef struct	s_data {
	void	*img;
	char	*addr;
	int     *data;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_info
{
	t_data	img;
	t_player p;
	void	*mlx;
	void	*mlx_win;
	char	**map;
	int		row;
	int		col;
	int		fd;
	int		C;
	int		px;
	int		py;
	int		ex;
	int		ey;
	int		tC;
	int		*cx;
	int		*cy;
}				t_info;


void	check_map(t_info *info);
void get_location_C(t_info *info);
bool	is_map_surrounded(t_info *info, int x, int y);
void	search_location_object(char **map, t_info *info);
void	get_location_object(t_info *info, char**map, int y, int x);
void	read_map(t_info *info);
int	find_chr(const char *s1,const char *s2);
void	free_line(t_info *info, int ret, char *line);
bool set_map(char *line, t_info *info);
void is_map_square(char **buf, t_info *info);
void init_info(t_info *info);
bool	freei_return(char **ptr, int n, bool ret);


#endif