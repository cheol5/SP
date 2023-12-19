#ifndef DEQUE_H
#define DEQUE_H

# include "Thread.h"

typedef struct node
{
	struct node	*next;
	struct node	*prev;
	Thread		*data;
}				t_node;

typedef struct deque
{
	t_node	*top;
	t_node	*bottom;
	int		cnt;
}			t_deque;

int		is_empty(t_deque *deque);
void	append(t_deque *deque, Thread data);
Thread	pop_left(t_deque *deque);
void	append_left(t_deque *deque, Thread data);
Thread	pop(t_deque *deque);


#endif