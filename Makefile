NAME = philo

CC = gcc

CFLAGS = -Wall -Wextra -Werror

INCLUDES = -I.

SRCS = getters_setters.c main.c dinner.c init.c init2.c monitoring.c parsing.c safe_functions.c synchro_utils.c utils.c write_status.c

OBJS = $(SRCS:.c=.o)

OBJ_DIR = ./objs/

OBJS_PREF = $(addprefix $(OBJ_DIR), $(OBJS))

all: $(NAME)

$(NAME): $(OBJS_PREF)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS_PREF)

$(OBJ_DIR)%.o: %.c philo.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@

debug: CFLAGS += -fsanitize=address -g
debug: re

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug
