#ifndef DEQUE_H
#define DEQUE_H

#include "Thread.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

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
void	append(t_deque *deque, Thread *data);
Thread	*pop_left(t_deque *deque);
void	append_left(t_deque *deque, Thread *data);
Thread	*pop(t_deque *deque);

void __thread_to_ready2(Thread *pTh);
void __thread_to_ready(int signo);
void __thread_to_run(Thread* pTh);
void __thread_to_zombie(thread_t tid);
t_node *findTcbBlock(thread_t tid, t_deque *deque);
t_node *getTcbBlock(thread_t tid);
Thread *removeTcbBlock(thread_t tid, t_deque *queue);

// deque
extern t_deque readyQueue;
extern t_deque waitQueue;
extern Thread *currentTcb;
extern pthread_mutex_t readyQueueMutex;
extern pthread_mutex_t waitQueueMutex;
extern int joinCnt;

#endif