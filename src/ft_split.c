#include"libft.h"

static size_t	count_strs(const char *str, char c)
{
	size_t	i;
	size_t	cnt;
	int		flag;

	i = 0;
	cnt = 0;
	flag = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			flag = 0;
		else if (flag == 0)
		{
			cnt++;
			flag = 1;
		}
		i++;
	}
	return (cnt);
}

static void	my_strcpy(char *dest, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

static char	**all_free(char **dest, t_size size)
{
	int	i;

	i = 0;
	while (i < (int)size.str_nbr)
	{
		free(dest[i]);
		i++;
	}
	free(dest);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	t_size	size;
	char	**dest;

	dest = malloc(sizeof(char *) * (count_strs(s, c) + 1));
	if (!s || !dest)
		return (NULL);
	size.i = 0;
	size.str_nbr = 0;
	while (s[size.i] != '\0' && size.str_nbr < count_strs(s, c))
	{
		while (s[size.i] != '\0' && s[size.i] == c)
			size.i++;
		size.len = 0;
		while (s[size.i + size.len] != '\0' && s[size.i + size.len] != c)
			size.len++;
		dest[size.str_nbr] = malloc(sizeof(char) * (size.len + 1));
		if (!dest[size.str_nbr])
			return (all_free(dest, size));
		my_strcpy(dest[size.str_nbr], &s[size.i], size.len + 1);
		size.str_nbr++;
		size.i += size.len;
	}
	dest[size.str_nbr] = (NULL);
	return (dest);
}
