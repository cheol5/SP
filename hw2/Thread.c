#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "deque.h"

t_deque		readyQueue = {0};
t_deque		waitQueue = {0};

Thread*     ReadyQHead = NULL;
Thread*     ReadyQTail = NULL;

Thread*     WaitQHead = NULL;
Thread*     WaitQTail = NULL;

pthread_mutex_t readyMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t readyCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t zombieCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t zombieMutex = PTHREAD_MUTEX_INITIALIZER;

void tcbBlockInit(Thread *block, thread_t *tid)
{
	block->status = THREAD_STATUS_READY;
	block->tid = *tid;
	block->readyCond = readyCond;
	block->bRunnable = 0;
	block->readyMutex = readyMutex;
	block->parentTid = pthread_self();
	block->zombieCond = zombieCond;
	block->zombieMutex = zombieMutex;
	block->bZombie = 1;
}

int 	thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	//tcb 생성해야함.
	Thread tcbBlock;
	WrapperArg args;
	args.funcPtr = start_routine;
	args.funcArg = arg;
	args.pThread = pthread_self();
	pthread_create(thread, 0, start_routine, arg);
	tcbBlockInit(&tcbBlock, thread);
	return 0;
}

t_node *findTcbBlock(thread_t tid){
	t_node *node = readyQueue.top;
	while (node){
		if (node->data->tid == tid)
			return node;
		node = node->next;
	}
	return 0;
}

int 	thread_join(thread_t thread, void **retval)
{

}


int 	thread_suspend(thread_t tid)
{
	t_node *node = findTcbBlock(tid);
	Thread *data;
	if (!node)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		data = node->data;
		free(node);
		waitQueue.append_left(&waitQueue, data);
		return 0;
	}
	return -1;
}


int	thread_resume(thread_t tid)
{
	t_node *node = findTcbBlock(tid);
	Thread *data;
	if (!node)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		data = node->data;
		free(node);
		readyQueue.append_left(&readyQueue, data);
		return 0;
	}
	return -1;
}

int             thread_exit(void* retval)
{
	pthread_exit(retval);
	return 0;
}


thread_t	thread_self()
{
	return pthread_self();
}
