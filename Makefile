# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysaadaou <ysaadaou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/05 14:12:16 by ysaadaou          #+#    #+#              #
#    Updated: 2025/02/27 14:44:30 by ysaadaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_DIR = ./srcs

BUILTINS_DIR = ./builtins/

SRCS = minishell.c \
	minishell_init.c \
	free_functions.c \
	parsing.c \
	command_functions.c \
	env_functions.c \
	utils_functions.c \
	lexer.c \
	lexer_utils.c \
	test_functions.c \
	# $(BUILTINS_DIR)builtins_utils.c \
	# $(BUILTINS_DIR)ft_cd.c \
	# $(BUILTINS_DIR)ft_echo.c \
	# $(BUILTINS_DIR)ft_env.c \
	# $(BUILTINS_DIR)ft_exit.c \
	# $(BUILTINS_DIR)ft_export.c \
	# $(BUILTINS_DIR)ft_pwd.c \
	# $(BUILTINS_DIR)ft_unset.c \
	# $(BUILTINS_DIR)main_builtins.c \

HEAD = minishell.h

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all
