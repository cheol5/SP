# Simple makefile.

NAME	=	hw1
CC		=	cc
SRCS	=	testcase.c\
			hw1.c\

#CFLAG	=	-Wall -Wextra -Werror
DFLAG	=	-g
OBJS	=	$(SRCS:%.c=%.o)

all		:	$(NAME)
	make clean

$(NAME)	:	$(OBJS)
	$(CC) -o $@ $^

%.o		:	%.c
	$(CC) -c $< -o $@

clean	:
	rm -rf $(OBJS)

fclean	:
	make clean
	rm -rf $(NAME)
	rm -rf storage

re		:
	make fclean
	make all

debug	:
	make fclean
	gcc -g $(SRCS)
	lldb a.out

.PHONY	: all clean fclean re debug
