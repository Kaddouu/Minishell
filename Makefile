# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/05 14:12:16 by ysaadaou          #+#    #+#              #
#    Updated: 2025/03/19 12:20:42 by ilkaddou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_DIR = ./srcs

BUILTINS_FILES = ./builtins/ft_cd.c \
	./builtins/ft_echo.c \
	./builtins/ft_env.c \
	./builtins/ft_exit.c \
	./builtins/ft_export.c \
	./builtins/ft_pwd.c \
	./builtins/ft_unset.c \
	./builtins/builtins_utils.c

PARSING_FILES = ./parsing/parsing.c \
	./parsing/command_functions.c \
	./parsing/env_functions.c \
	./parsing/lexer.c \
	./parsing/lexer_utils.c

EXEC_FILES = ./exec/exec_functions.c \
	./exec/exec_utils.c

SRCS = minishell.c \
	minishell_init.c \
	free_functions.c \
	signals.c \
	$(BUILTINS_FILES) \
	$(PARSING_FILES) \
	$(EXEC_FILES)

HEAD = minishell.h

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Werror -Wextra -g

# Détecter le système d'exploitation
UNAME := $(shell uname)

# Configuration spécifique à Linux
ifeq ($(UNAME), Linux)
	# Vérifier si pkg-config est installé et fonctionne pour readline
	READLINE_DIR := $(shell pkg-config --variable=prefix readline)
	ifeq ($(strip $(READLINE_DIR)),)
	$(error "pkg-config failed to find readline. Make sure readline is installed.")
	endif
	CFLAGS += -I$(READLINE_DIR)/include
	LDFLAGS += -L$(READLINE_DIR)/lib -lreadline
endif

# Configuration spécifique à macOS
ifeq ($(UNAME), Darwin)
	READLINE_DIR := $(shell brew --prefix readline)
	ifeq ($(strip $(READLINE_DIR)),)
	$(error "Homebrew readline not found. Make sure Homebrew is installed.")
	endif
	CFLAGS += -I$(READLINE_DIR)/include
	LDFLAGS += -L$(READLINE_DIR)/lib -lreadline
endif

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all
