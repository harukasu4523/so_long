/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 16:08:21 by hiwata            #+#    #+#             */
/*   Updated: 2021/02/04 21:15:08 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

int	read_fd(char **str, int fd)
{
	char			*buf;
	char			*tmp;
	ssize_t			r_size;

	r_size = 1;
	*str = ft_strdup("");
	while (r_size && !ft_strrchr(*str, '\n'))
	{
		buf = malloc(BUFFER_SIZE + 1);
		if (!buf)
			return (-1);
		r_size = read(fd, buf, BUFFER_SIZE);
		if (r_size < 0)
			return (-1);
		buf[r_size] = '\0';
		tmp = ft_strjoin(*str, buf);
		free(*str);
		free(buf);
		*str = ft_strdup(tmp);
		free(tmp);
		if (!str)
			return (-1);
	}
	return (r_size);
}

ssize_t	str_memo(char **str, char **memo, int fd, int *flag)
{
	char			*p_newl;
	char			*tmp;
	ssize_t			len;

	if (memo[fd])
	{
		tmp = ft_strjoin(memo[fd], *str);
		free(memo[fd]);
		memo[fd] = NULL;
		free(*str);
		*str = ft_strdup(tmp);
		free(tmp);
		if (!str)
			return (-1);
	}
	p_newl = ft_strrchr(*str, '\n');
	if (p_newl)
		*flag = 1;
	if (!p_newl)
		p_newl = ft_strrchr(*str, '\0');
	if (p_newl)
		len = p_newl - *str;
	else
		len = 0;
	if (!*flag)
		return (len);
	memo[fd] = ft_strdup(&str[0][len + 1]);
	if (!(memo[fd]))
		return (-1);
	return (len);
}

int	free_any_thing(char **str, char **memo, int fd, int flag)
{
	char	**f_param;

	f_param = str;
	if (str && flag != 0)
	{
		if (*f_param)
			free(*f_param);
		else
			*f_param = 0;
		free(str);
		return (-1);
	}
	if (flag > 0)
		return (0);
	free(memo[fd]);
	return (-1);
}

int	get_next_line(int fd, char **line)
{
	char			**str;
	static char		*memo[MAX_FD] = {0};
	int				flag[1];
	ssize_t			size;

	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0)
		return (-1);
	str = (char **)malloc(sizeof(char *));
	if (!str)
		return (-1);
	*flag = 0;
	if (read_fd(str, fd) < 0)
		return (free_any_thing(str, memo, fd, -1));
	size = str_memo(str, memo, fd, flag);
	if (size < 0)
		return (free_any_thing(str, memo, fd, -1));
	*line = NULL;
	*line = malloc(size + 1);
	if (!*line)
		return (free_any_thing(str, memo, fd, -1));
	ft_strlcpy(*line, *str, size + 1);
	free_any_thing(str, memo, fd, 1);
	if (!*flag)
		return (0);
	return (1);
}
