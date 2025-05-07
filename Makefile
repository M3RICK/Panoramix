##
## EPITECH PROJECT, 2025
## PANORAMIX
## File description:
## Makefile
##

NAME	= panoramix

CC	= gcc
RM	= rm -f

SRCS	= 	src/main.c	\
		  	src/threads_handler.c	\
			  src/tools.c
OBJS	= $(SRCS:.c=.o)

CFLAGS	= -Wall -Wextra -Werror -pthread
LDFLAGS	= -pthread

JINX_LOGO = "\033[1;35m💣 Time to crank up the chaos! - Jinx\033[0m"

all: $(NAME)
	@echo $(JINX_LOGO)

$(NAME): $(OBJS)
	@echo "\033[1;36m🔧 Assembling weapons...\033[0m"
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "\033[1;32m✅ $(NAME) built successfully! Pow-pow!\033[0m"

clean:
	@echo "\033[1;33m🧹 Cleaning up the mess...\033[0m"
	$(RM) $(OBJS)
	@echo "\033[1;32m✅ Objects obliterated.\033[0m"

fclean: clean
	@echo "\033[1;31m💥 Blowing everything to bits...\033[0m"
	$(RM) $(NAME)
	@echo "\033[1;32m✅ $(NAME) is no more. Boom!\033[0m"

re: fclean all

.PHONY: all clean fclean re
