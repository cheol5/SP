#ifndef DEQUE_H
#define DEQUE_H

# include "Thread.h"

typedef struct node
{
	struct node	*next;
	struct node	*prev;
	Thread		data;
}				t_node;

typedef struct deque
{
	t_node	*top;
	t_node	*bottom;
	int		cnt;
}			t_deque;

#endif