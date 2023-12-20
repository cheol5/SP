#include "deque.h"

int	is_empty(t_deque *deque)
{
	if (deque->cnt == 0)
		return (1);
	return (0);
}


void	append(t_deque *deque, Thread *data)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	node->next = 0;
	node->prev = 0;
	node->data = data;
	if (is_empty(deque))
		deque->top = node;
	else
	{
		node->prev = deque->bottom;
		deque->bottom->next = node;
	}
	deque->bottom = node;
	deque->cnt++;
}

Thread	*pop_left(t_deque *deque)
{
	t_node	*ptr;
	Thread	*data;

	if (is_empty(deque))
		exit(1);
	ptr = deque->top;
	data = ptr->data;
	if (deque->cnt == 1)
	{
		deque->top = 0;
		deque->bottom = 0;
	}
	else
	{
		deque->top = ptr->next;
		deque->top->prev = 0;
	}
	deque->cnt--;
	free(ptr);
	return (data);
}

void	append_left(t_deque *deque, Thread *data)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	node->next = 0;
	node->prev = 0;
	node->data = data;
	if (is_empty(deque))
		deque->bottom = node;
	else
	{
		node->next = deque->top;
		deque->top->prev = node;
	}
	deque->top = node;
	deque->cnt++;
}

Thread	*pop(t_deque *deque)
{
	t_node	*ptr;
	Thread	*data;

	if (is_empty(deque))
		exit(1);
	ptr = deque->bottom;
	data = ptr->data;
	if (deque->cnt == 1)
	{
		deque->top = 0;
		deque->bottom = 0;
	}
	else
	{
		deque->bottom = ptr->prev;
		deque->bottom->next = 0;
	}
	deque->cnt--;
	free(ptr);
	return (data);
}

t_node *findTcbBlock(thread_t tid, t_deque *deque)
{
	t_node *node = deque->top;
	while (node){
		if (node->data->tid == tid)
			return node;
		node = node->next;
	}
	return 0;
}

void __thread_to_ready2(Thread *pTh)
{
	pthread_mutex_lock(&(pTh->readyMutex));
	while (pTh->bRunnable == FALSE)
		pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
	pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_to_zombie(thread_t tid)
{
	t_node *node = findTcbBlock(tid, &readyQueue);
	if (!node)
		node = findTcbBlock(tid, &waitQueue);
	Thread *tcb = node->data;
	tcb->status = THREAD_STATUS_ZOMBIE; // change status

	pthread_mutex_lock(&(tcb->zombieMutex));
	tcb->bZombie == TRUE;
	pthread_cond_signal(&(tcb->zombieCond));
	pthread_mutex_unlock(&(tcb->zombieMutex));
}