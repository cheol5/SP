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

static void tcbBlockInit(Thread *block)
{
	block->status = THREAD_STATUS_READY;
	block->readyCond = readyCond;
	block->tid = 0;
	block->bRunnable = 0;
	block->readyMutex = readyMutex;
	block->parentTid = pthread_self(); // pthread_create호출 전이므로 parent's tid 가 들어간다. 
	block->zombieCond = zombieCond;
	block->zombieMutex = zombieMutex;
	block->bZombie = 1;
}

static void *wrapperFunc(void *arg){
	
	void *ret;
	WrapperArg *pArg = (WrapperArg *)arg;
	void *funcPtr = pArg->funcPtr;
	void *funcArg = pArg->funcArg;
	Thread *pTh = pArg->pThread;
	pTh->tid = pthread_self();
	printf("thread id is %d, parent :%d\n", (int)pTh->tid, (int)pTh->parentTid);
	append_left(&readyQueue, pTh); // ready queue에 삽입.
	printf("ready queue size: %d\n", readyQueue.cnt);
	t_node *node = readyQueue.top;
	while (node)
	{
		printf("traversal tid :%d\n", (int)(node->data->tid));
		node = node->next;
	}
	// pthread_mutex_unlock(&queueMutex);
	__thread_to_ready2(pTh); // 생성된 쓰레드를 최초 1회 ready2함수를 호출한다. 
	ret = ((void *(*)(void *))funcPtr)(funcArg);
	// free(arg);
	return ret;
}



int 	thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	//tcb 생성해야함.
	// pthread_mutex_lock(&queueMutex);
	Thread *tcbBlock = (Thread *)malloc(sizeof(Thread));
	WrapperArg *args = (WrapperArg *)malloc(sizeof(WrapperArg));

	tcbBlockInit(tcbBlock);
	args->funcPtr = start_routine;
	args->funcArg = arg;
	args->pThread = tcbBlock;
	pthread_create(thread, 0, wrapperFunc, args);
	return 0;
}

int 	thread_join(thread_t thread, void **retval)
{
	t_node *node = getTcbBlock(thread);
	Thread *tcb = node->data;

	pthread_mutex_lock(&(tcb->zombieMutex));
	while (tcb->bZombie == FALSE)
		pthread_cond_wait(&(tcb->zombieCond), &(tcb->zombieMutex));
	pthread_mutex_unlock(&(tcb->zombieMutex));
	// thread_exit(*retval);
	*retval = tcb->pExitCode;
	return 0;
}

int 	thread_suspend(thread_t tid)
{
	t_node *node = findTcbBlock(tid, &readyQueue);
	Thread *data;
	if (!node)
	{
		if (!node->prev)
			node->prev->next = node->next;
		if (!node->next)
			node->next->prev = node->prev;
		data = node->data;
		data->status = THREAD_STATUS_BLOCKED; // Change status.
		free(node);
		append_left(&waitQueue, data);
		return 0;
	}
	return -1;
}

int	thread_resume(thread_t tid)
{
	t_node *node = findTcbBlock(tid, &waitQueue);
	Thread *data;
	if (!node)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		data = node->data;
		data->status = THREAD_STATUS_READY;
		free(node);
		append_left(&readyQueue, data);
		return 0;
	}
	return -1;
}

int             thread_exit(void* retval)
{
	__thread_to_zombie(pthread_self());
	t_node *node = getTcbBlock(pthread_self());
	Thread *data = node->data;
	data->pExitCode = retval;
	return 0;
}

thread_t	thread_self()
{
	return pthread_self();
}
