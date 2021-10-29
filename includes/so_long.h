/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 23:31:44 by hiwata            #+#    #+#             */
/*   Updated: 2021/10/29 22:33:48 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdio.h>
# include <stdbool.h>
# include <mlx.h>
# include "get_next_line.h"
# include "libft.h"

# define TILE_SIZE 30
# define M_UP 13
# define M_DOWN 1
# define M_LEFT 0
# define M_RIGHT 2
# define M_ESC 53
# define UP 119
# define DOWN 115
# define LEFT 97
# define RIGHT 100
# define ESC 65307
# define WALL 49
# define END 101
# define OBJECT 71
# define FLOOR 42

typedef struct s_tex
{
	int	texture[5][4096];
	int	width[5];
	int	height[5];

}				t_tex;

typedef struct s_player
{
	int	move_y;
	int	move_x;
}				t_player;

typedef struct s_data {
	void	*img;
	char	*addr;
	int		*data;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_data;

typedef struct s_info
{
	t_data		img;
	t_player	p;
	t_tex		tex;
	void		*mlx;
	void		*mlx_win;
	char		**map;
	int			win_width;
	int			win_height;
	int			row;
	int			col;
	int			fd;
	int			c;
	int			px;
	int			py;
	int			ex;
	int			ey;
	int			tc;
	int			*cx;
	int			*cy;
	int			move_cnt;
	bool		cnt_flag;
}				t_info;

void	check_map(t_info *info);
void	get_location_C(t_info *info);
bool	is_map_surrounded(t_info *info, int x, int y);
void	search_location_object(char **map, t_info *info);
void	get_location_object(t_info *info, char**map, int y, int x);
void	read_map(t_info *info);
int		find_chr(const char *s1, const char *s2);
void	free_line(t_info *info, int ret, char *line);
void	free_texture(t_info *info, int tex_num);
bool	set_map(char *line, t_info *info);
void	is_map_square(char **buf, t_info *info);
void	init_info(t_info *info);
bool	freei_return(char **ptr, int n, bool ret);
#endif