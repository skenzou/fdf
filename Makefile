# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/29 10:58:01 by midrissi          #+#    #+#              #
#    Updated: 2019/02/15 06:26:26 by midrissi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf
SRCS = srcs/fdf.c srcs/read.c
OBJDIR = ./obj/
OBJ = $(patsubst srcs/%.c,$(OBJDIR)%.o,$(SRCS))
CFLAGS = -Wall -Wextra -Werror -Iincludes
MLX_PATH = ./minilibx
MLX_LINK = -L $(MLX_PATH) -lmlx -framework OpenGL -framework AppKit
MLX_INC = -I $(MLX_PATH)
LPATH = ./libft/
LIB = $(addprefix $(LPATH),libft.a)

all: $(NAME)

$(NAME): $(OBJDIR)
	@make -C $(MLX_PATH)
	@make -C $(LPATH)
	@gcc $(CFLAGS) -o $(NAME) $(LIB) $(OBJ) $(MLX_LINK)


$(OBJDIR): $(SRCS)
	@gcc $(CFLAGS) -c $(SRCS) $(MLX_INC)
	@mkdir $(OBJDIR)
	@mv $(notdir $(OBJ)) $(OBJDIR)


clean:
	@rm -f $(OBJ)
	@rm -rf $(OBJDIR)
	@make clean -C $(MLX_PATH)
	@make fclean -C $(LPATH)

fclean: clean
	@rm -f $(NAME)
	@make clean -C $(MLX_PATH)
	@make fclean -C $(LPATH)

re: fclean all

.PHONY: all clean fclean re
