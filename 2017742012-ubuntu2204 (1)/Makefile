# Simple makefile.

NAME	=	hw2
CC		=	gcc
SRCS	=	main.c\
			Scheduler.c\
			Thread.c\
			deque.c\
			init.c\
			TestCase1.c\
			TestCase2.c\
			TestCase3.c\

#CFLAG	=	-Wall -Wextra -Werror
DFLAG	=	-g
# FFLAG	=	-fsanitize=address
OBJS	=	$(SRCS:%.c=%.o)

all		:	$(NAME)
	make clean

$(NAME)	:	$(OBJS)
	$(CC) $(FFLAG) -o $@ $^

%.o		:	%.c
	$(CC) -c $< -o $@

clean	:
	rm -rf $(OBJS)

fclean	:
	make clean
	rm -rf $(NAME)

re		:
	make fclean
	make all

debug	:
	make fclean
	gcc -g $(SRCS)
	lldb a.out

.PHONY	: all clean fclean re debug
