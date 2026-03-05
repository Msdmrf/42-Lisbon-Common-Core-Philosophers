# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/02 10:20:02 by migusant          #+#    #+#              #
#    Updated: 2026/03/05 13:20:39 by migusant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                              PROJECT SETTINGS                                #
# **************************************************************************** #

NAME = philo
ARGS = 2 410 200 200 5

# **************************************************************************** #
#                              COMPILER SETTINGS                               #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

# **************************************************************************** #
#                             DIRECTORY STRUCTURE                              #
# **************************************************************************** #

INC_DIR = inc/
SRC_DIR = src/
OBJ_DIR = obj/
LOG_DIR = log/

# **************************************************************************** #
#                                  COLORS                                      #
# **************************************************************************** #

GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

# **************************************************************************** #
#                              SOURCE FILES                                    #
# **************************************************************************** #

SRC = src/main.c \
	src/signals.c \
	src/parser.c \
	src/init.c \
	src/routine.c \
	src/monitor.c \
	src/utils.c

OBJ = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

# **************************************************************************** #
#                                 TARGETS                                      #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW)Building $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(NAME) successfully created!$(RESET)"

v: all
	@mkdir -p $(LOG_DIR)
	@clear && valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --log-file=$(LOG_DIR)valgrind.log \
		-s ./$(NAME) $(ARGS)
	@echo && cat $(LOG_DIR)valgrind.log

h: all
	@mkdir -p $(LOG_DIR)
	@clear && valgrind --tool=helgrind --log-file=$(LOG_DIR)helgrind.log \
		-s ./$(NAME) $(ARGS)
	@echo && cat $(LOG_DIR)helgrind.log

# **************************************************************************** #
#                            COMPILATION RULES                                 #
# **************************************************************************** #

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling: $<$(RESET)"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# **************************************************************************** #
#                              CLEANING RULES                                  #
# **************************************************************************** #

clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		$(RM) -r $(OBJ_DIR); \
		echo "$(RED)Object files have been cleaned!$(RESET)"; \
		echo "$(YELLOW)└── Removed directory: $(OBJ_DIR)$(RESET)"; \
	else \
		echo "$(BLUE)No object files to clean.$(RESET)"; \
	fi
	@if [ -d "$(LOG_DIR)" ]; then \
		$(RM) -r $(LOG_DIR); \
		echo "$(RED)Log files have been cleaned!$(RESET)"; \
		echo "$(YELLOW)└── Removed directory: $(LOG_DIR)$(RESET)"; \
	else \
		echo "$(BLUE)No log files to clean.$(RESET)"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		$(RM) $(NAME); \
		echo "$(RED)Executable file has been cleaned!$(RESET)"; \
		echo "$(YELLOW)└── Removed executable: $(NAME)$(RESET)"; \
	else \
		echo "$(BLUE)No executable file to clean.$(RESET)"; \
	fi

re: fclean all

.PHONY: all clean fclean re v h