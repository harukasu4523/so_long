/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 13:04:12 by hiwata            #+#    #+#             */
/*   Updated: 2021/02/04 21:15:26 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	i++;
	while (i)
	{
		if (*s == c)
		{
			return ((char *)s);
		}
		s++;
		i--;
	}
	return (NULL);
}

// char	*ft_strdup(const char *s1)
// {
// 	size_t			len;
// 	char			*p;
// 	char			*s_cpy;

// 	len = 0;
// 	while (s1[len] != '\0')
// 		len++;
// 	s_cpy = malloc(sizeof(char) * (len + 1));
// 	if (!s_cpy)
// 	{
// 		free(s_cpy);
// 		return (NULL);
// 	}
// 	else
// 	{
// 		p = s_cpy;
// 		while (*s1)
// 			*p++ = *s1++;
// 		*p = '\0';
// 		return (s_cpy);
// 	}
// }

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t			d_len;
	size_t			s_len;
	size_t			i;

	d_len = 0;
	while (dst[d_len])
		d_len++;
	s_len = 0;
	while (src[s_len])
		s_len++;
	i = 0;
	if (d_len < dstsize)
	{
		while (dstsize - d_len - 1 > i && src[i])
		{
			dst[d_len + i] = src[i];
			i++;
		}
	}
	dst[i + d_len] = '\0';
	if (d_len >= dstsize)
		return (s_len + dstsize);
	return (s_len + d_len);
}

void	ft_strlcpy(char *dest, char *src, size_t size)
{
	char			*pdst;
	const char		*psrc;
	size_t			i;

	pdst = dest;
	psrc = src;
	i = size;
	if (i != 0)
	{
		while (--i != 0)
		{
			if (*psrc == '\0')
				break ;
			*pdst++ = *psrc++;
		}
	}
	if (i == 0)
	{
		if (size != 0)
			*pdst = '\0';
	}
}
