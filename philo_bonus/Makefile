# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/17 15:30:49 by migusant          #+#    #+#              #
#    Updated: 2026/03/12 14:22:00 by migusant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                              PROJECT SETTINGS                                #
# **************************************************************************** #

NAME = philo_bonus

# *** Evaluation Sheet Tests: *** #

#ARGS = 1 800 200 200
#ARGS = 5 800 200 200
ARGS = 5 800 200 200 7
#ARGS = 4 410 200 200
#ARGS = 4 310 200 100
#ARGS = 2 410 200 200
#ARGS = 2 310 200 100

# *** Extra Tests: *** #

# --- (+1ms) --- #
#ARGS = 3 601 200 200
#ARGS = 3 601 200 100

# --- (+5ms) --- #
#ARGS = 49 800 200 200 10
#ARGS = 49 605 200 200
#ARGS = 49 605 200 100

# --- (+5ms) --- #
#ARGS = 50 800 200 200 10
#ARGS = 50 405 200 200
#ARGS = 50 401 200 100

# --- (+10ms) --- #
#ARGS = 99 800 200 200 10
#ARGS = 99 610 200 200
#ARGS = 99 610 200 100

# --- (+10ms) --- #
#ARGS = 100 800 200 200 10
#ARGS = 100 420 200 200
#ARGS = 100 420 200 100

# --- (+15ms) --- #
#ARGS = 149 800 200 200 10
#ARGS = 149 615 200 200
#ARGS = 149 615 200 100

# --- (+20ms) --- #
#ARGS = 150 800 200 200 10
#ARGS = 150 420 200 200
#ARGS = 150 420 200 100

# --- (+20ms) --- #
#ARGS = 199 800 200 200 10
#ARGS = 199 620 200 200
#ARGS = 199 620 200 100

# --- (+30ms) --- #
#ARGS = 200 800 200 200 10
#ARGS = 200 430 200 200
#ARGS = 200 430 200 100

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
	src/parser.c \
	src/init.c \
	src/processes.c \
	src/routine.c \
	src/forks.c \
	src/monitor.c \
	src/cleanup.c \
	src/signals.c \
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

t: all
	@timeout --foreground 60 ./$(NAME) $(ARGS); \
	result=$$?; \
	if [ $$result -eq 124 ]; then \
		echo "(Timeout [60s]: $(ARGS))"; \
	fi

v: all
	@mkdir -p $(LOG_DIR)
	@clear && valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --track-fds=yes \
		--log-file=$(LOG_DIR)valgrind.log -s ./$(NAME) $(ARGS)
	@echo && cat $(LOG_DIR)valgrind.log

vp: all
	@mkdir -p $(LOG_DIR)
	@clear && valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --track-fds=yes --trace-children=yes \
		--log-file=$(LOG_DIR)valgrind_%p.log -s ./$(NAME) $(ARGS)
	@echo && cat $(LOG_DIR)valgrind_*.log

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
	@rm -f /dev/shm/sem.philo_* 2>/dev/null || true

re: fclean all

.PHONY: all clean fclean re t v vp