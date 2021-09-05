# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hiwata <hiwata@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/31 19:48:29 by hiwata            #+#    #+#              #
#    Updated: 2021/09/01 13:41:15 by hiwata           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = so_long

SRCS_DIR = ./src/

SRCS_NAME = ft_atoi.c ft_memset.c ft_split.c ft_strchr.c ft_strcmp.c ft_strdup.c ft_strjoin.c ft_strlen.c get_next_line.c get_next_line_utils.c \
			 readfile.c map.c

SRCS = ${addprefix ${SRCS_DIR}, ${SRCS_NAME}}

HEADER = ./includes

MLX_DIR = ./minilibx_mac/minilibx

OBJS = ${SRCS:.c=.o}

CC = gcc

CFLAG = -Wall -Wextra -Werror -I ${HEADER} -I ${MLX_DIR}

RM = rm -rf

all:	${NAME}

.c.o:
		$(CC) $(CFLAG) -c $< -o ${<:.c=.o}

# gcc -L minilibx -lmlx -framework OpenGL -framework AppKit src/*.c -I include

${NAME}:	${OBJS} ${HEADER}
			@${MAKE} -C ${MLX_DIR}
			${CC} ${CFLAG} -o ${NAME} ${OBJS} -L ${MLX_DIR} -lmlx -framework OpenGL  -framework AppKit

clean:
		${RM} ${OBJS}

fclean:	clean
		${RM} ${NAME}

re:	fclean all

.PHONY: all clean fclean re