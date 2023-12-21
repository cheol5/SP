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

Thread *removeTcbBlock(thread_t tid, t_deque *queue)
{
	t_node *node = getTcbBlock(tid);
	Thread *data;
	if (node)
	{
		if (!node->prev)
			return pop_left(queue);
		if (!node->next)
			return pop(queue);
		//가운데에 있는 케이스
		data = node->data;
		queue->cnt--;
		node->prev->next = node->next;
		node->next->prev = node->prev;
		data->status = THREAD_STATUS_BLOCKED; // Change status.
		free(node);
		return data;
	}
	return 0;
}

t_node *getTcbBlock(thread_t tid)
{
	t_node *node = findTcbBlock(tid, &readyQueue);
	if (!node)
		node = findTcbBlock(tid, &waitQueue);
	return node;
}

void __thread_to_zombie(thread_t tid)
{
	t_node *node = getTcbBlock(tid);
	Thread *tcb = node->data;
	tcb->status = THREAD_STATUS_ZOMBIE; // change status
	
	pthread_mutex_lock(&(tcb->zombieMutex));
	tcb->bZombie = TRUE;
	pthread_cond_signal(&(tcb->zombieCond));
	pthread_mutex_unlock(&(tcb->zombieMutex));
}

// time slice만큼 동작이 끝난 쓰레드를 ready상태로 만들어주는 함수
void __thread_to_ready(int signo) {
    Thread *pTh = currentTcb;
    pthread_mutex_lock(&(pTh->readyMutex));
    while (pTh->bRunnable == FALSE)
        pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
    pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_to_run(Thread* pTh)
{
	pthread_mutex_lock(&(pTh->readyMutex));
	pTh->bRunnable = TRUE;
	pthread_cond_signal(&(pTh->readyCond));
	pthread_mutex_unlock(&(pTh->readyMutex));
}
