#include "libft.h"

// 99999999999999 でオーバーフロー
// 時間あるとき直せよ

int	ft_atoi(const char *str)
{
	int				sign;
	unsigned long	result;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	result = 0;
	while ('0' <= *str && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result > LONG_MAX && sign == 1)
			return (-1);
		if (result > (unsigned long)-LONG_MIN && sign == -1)
			return (0);
		str++;
	}
	return (result * sign);
}
