# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/09 12:43:35 by midrissi          #+#    #+#              #
#    Updated: 2019/02/20 06:23:08 by midrissi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRCS = srcs/ft_atoi.c srcs/ft_isalnum.c srcs/ft_bzero.c srcs/ft_isalpha.c \
srcs/ft_isascii.c srcs/ft_isdigit.c srcs/ft_isprint.c srcs/ft_itoa.c \
srcs/ft_lstadd.c srcs/ft_lstdel.c srcs/ft_lstdelone.c srcs/ft_lstiter.c \
srcs/ft_lstmap.c srcs/ft_lstnew.c srcs/ft_memalloc.c srcs/ft_memccpy.c \
srcs/ft_memchr.c srcs/ft_memcmp.c srcs/ft_memcpy.c srcs/ft_memdel.c \
srcs/ft_memmove.c srcs/ft_memset.c srcs/ft_putchar.c srcs/ft_putchar_fd.c \
srcs/ft_putendl.c srcs/ft_putendl_fd.c srcs/ft_putnbr.c srcs/ft_putnbr_fd.c \
srcs/ft_putstr.c srcs/ft_putstr_fd.c srcs/ft_strcat.c srcs/ft_strchr.c \
srcs/ft_strclr.c srcs/ft_strcmp.c srcs/ft_strcpy.c srcs/ft_strdel.c \
srcs/ft_strdup.c srcs/ft_strequ.c srcs/ft_striter.c srcs/ft_striteri.c \
srcs/ft_strjoin.c srcs/ft_strlcat.c srcs/ft_strlen.c srcs/ft_strmap.c \
srcs/ft_strtrim.c srcs/ft_strsplit.c srcs/ft_strstr.c srcs/ft_strsub.c \
srcs/ft_toupper.c srcs/ft_strmapi.c srcs/ft_strncat.c srcs/ft_strncmp.c \
srcs/ft_strncpy.c srcs/ft_strnequ.c srcs/ft_strnew.c srcs/ft_strnstr.c \
srcs/ft_strrchr.c srcs/ft_tolower.c srcs/get_next_line.c srcs/ft_itoa_base.c \
srcs/ft_nputchar.c srcs/ft_ftoa.c srcs/ft_utoa_base.c srcs/ft_count_char.c \
srcs/ft_lstpushback.c srcs/ft_lstrev.c srcs/ft_lstdestroy.c \
srcs/ft_count_words.c srcs/ft_printf.c srcs/numbers.c srcs/format_geters.c \
srcs/handlers.c

OBJDIR = ./obj/
OBJ = $(patsubst srcs/%.c,$(OBJDIR)%.o,$(SRCS))
COMP = gcc
CFLAGS = -Wall -Wextra -Werror -I./includes -c
AR = ar rcs

all: $(NAME)

$(NAME): $(OBJDIR)
	@$(AR) $(NAME) $(OBJ)

$(OBJDIR): $(SRCS)
	@$(COMP) $(CFLAGS) $(SRCS)
	@mkdir $(OBJDIR)
	@mv $(notdir $(OBJ)) $(OBJDIR)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
