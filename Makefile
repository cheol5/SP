# Simple makefile.

NAME	=	a.out
CC		=	cc
SRCS	=	main.c\
			src.c\

CFLAG	=	-Wall -Wextra -Werror
FFLAG	=	-fsanitize=address
OBJS	=	$(SRCS:%.c=%.o)

all		:	$(NAME)

$(NAME)	:	$(OBJS)
	$(CC) $(CFLAG) $(FFLAG) -o $@ $^

%.o		:	%.c
	$(CC) $(CFLAG) $(FFLAG) -c $< -o $@

clean	:
	rm -rf $(OBJS)

fclean	:
	make clean
	rm -rf $(NAME)

re		:
	make fclean
	make all

.PHONY	: all clean fclean re
