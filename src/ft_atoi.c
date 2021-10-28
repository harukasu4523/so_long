/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 20:17:18 by hiwata            #+#    #+#             */
/*   Updated: 2021/10/28 20:18:10 by hiwata           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *s)
{
	long long	nb;
	int			flag;

	nb = 0;
	flag = 1;
	while (*s == '\t' || *s == '\n' || *s == '\v' || *s == '\f' \
	|| *s == '\r' || *s == ' ')
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			flag *= -1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		nb *= 10;
		nb += (long long)*s - '0';
		s++;
	}
	return ((int)(nb * flag));
}