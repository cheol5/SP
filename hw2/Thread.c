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
int joinCnt = 0;

static void tcbBlockInit(Thread *block)
{
	block->status = THREAD_STATUS_READY;
	block->readyCond = readyCond;
	block->tid = 0;
	block->pExitCode = 0;
	block->bRunnable = 0;
	block->readyMutex = readyMutex;
	block->parentTid = pthread_self(); // pthread_create호출 전이므로 parent's tid 가 들어간다. 
	block->zombieCond = zombieCond;
	block->zombieMutex = zombieMutex;
	block->bZombie = 0;
}

static void *wrapperFunc(void *argT)
{
	WrapperArg *pArg = (WrapperArg *)argT;
	Thread *pTh = pArg->pThread;
	pTh->tid = pthread_self();
	// printf("thread id is %d, parent :%d\n", (int)pTh->tid, (int)pTh->parentTid);
	append_left(&readyQueue, pTh); // ready queue에 삽입.
	// printf("ready queue size: %d\n", readyQueue.cnt);
	// t_node *node = readyQueue.top;
	// while (node)
	// {
	// 	printf("traversal tid :%d\n", (int)(node->data->tid));
	// 	node = node->next;
	// }
	__thread_to_ready2(pTh); // 생성된 쓰레드를 최초 1회 ready2함수를 호출한다. 
	return pArg->funcPtr(pArg->funcArg);
	// free(argT);
}



int 	thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	//tcb 생성해야함.
	Thread *tcbBlock = (Thread *)malloc(sizeof(Thread));
	WrapperArg *args = (WrapperArg *)malloc(sizeof(WrapperArg));
	memset(tcbBlock, 0, sizeof(Thread));
	memset(args, 0, sizeof(WrapperArg));
	tcbBlockInit(tcbBlock);
	args->funcPtr = start_routine;
	args->funcArg = (void *)arg;
	args->pThread = tcbBlock;
	pthread_create(thread, 0, wrapperFunc, args);
	return 0;
}

int 	thread_join(thread_t thread, void **retval)
{
	t_node *node = getTcbBlock(thread);
	Thread *nodeTcb = node->data;
	t_node *mainNode = getTcbBlock(node->data->parentTid);
	Thread *mainTcb = mainNode->data;
	
	// printf("main Tid is %d, joinCnt:%d\n", (int)mainTcb->tid, joinCnt);
	// 메인 스레드를(Testcase2) 웨이팅 queue에 넣는다.
	
	pthread_mutex_lock(&readyQueueMutex);
	if((mainNode = findTcbBlock(mainTcb->tid, &readyQueue)))
	{
		removeTcbBlock(mainTcb->tid, &readyQueue);
		__thread_to_run(readyQueue.bottom->data);
		joinCnt = readyQueue.cnt;
		__thread_to_run(mainTcb);
		append_left(&waitQueue, mainTcb);
	}
	pthread_mutex_unlock(&readyQueueMutex);

	pthread_mutex_lock(&(nodeTcb->zombieMutex));
	while (nodeTcb->bZombie == FALSE)
		pthread_cond_wait(&(nodeTcb->zombieCond), &(nodeTcb->zombieMutex));
	pthread_mutex_unlock(&(nodeTcb->zombieMutex));
	*retval = (node->data->pExitCode);
	return 0;
}

int 	thread_suspend(thread_t tid)
{
	t_node *node = findTcbBlock(tid, &readyQueue);
	Thread *data;
	if (node)
	{
		// 맨 앞
		if (!node->prev)
		{
			data = pop_left(&readyQueue);
			append_left(&waitQueue, data);
			return 0;
		}
		// 맨 뒤
		if (!node->next)
		{
			data = pop(&readyQueue);
			append_left(&waitQueue, data);
			return 0;
		}
		//가운데에 있는 케이스
		data = node->data;
		node->prev->next = node->next;
		node->next->prev = node->prev;
		readyQueue.cnt--;
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
	if (node)
	{
		if (!node->prev)
		{
			data = pop_left(&waitQueue);
			append_left(&readyQueue, data);
			return 0;
		}
		if (!node->next)
		{
			data = pop(&waitQueue);
			append_left(&readyQueue, data);
			return 0;
		}
		//가운데에 있는 케이스
		data = node->data;
		node->prev->next = node->next;
		node->next->prev = node->prev;
		data->status = THREAD_STATUS_BLOCKED; // Change status.
		free(node);
		append_left(&readyQueue, data);
		return 0;
	}
	return -1;
}

int             thread_exit(void* retval)
{
	t_node *node = getTcbBlock(pthread_self());
	Thread *data = node->data;
	// printf("exit:%d\n", (int)data->tid);
	joinCnt--;
	__thread_to_zombie(data->tid);
	data->pExitCode = retval;
	// removeTcbBlock(data->tid, &readyQueue);
	// removeTcbBlock(data->tid, &waitQueue);
	return 0;
}

thread_t	thread_self()
{
	return pthread_self();
}
