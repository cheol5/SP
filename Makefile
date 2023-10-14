# Simple makefile.

NAME	=	assignment
CC		=	cc
SRCS	=	testcase.c\
			hw1.c\


CFLAG	=	-Wall -Wextra -Werror
#FFLAG	=	-fsanitize=address
OBJS	=	$(SRCS:%.c=%.o)

all		:	$(NAME)
	make clean

$(NAME)	:	$(OBJS)
	$(CC) $(FFLAG) -o $@ $^

%.o		:	%.c
	$(CC) $(FFLAG) -c $< -o $@

clean	:
	rm -rf $(OBJS)

fclean	:
	make clean
	rm -rf $(NAME)
	rm -rf storage

re		:
	make fclean
	make all

.PHONY	: all clean fclean re
