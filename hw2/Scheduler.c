#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "signal.h"
#include "deque.h"

Thread *currentTcb = 0;
pthread_mutex_t readyQueueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waitQueueMutex = PTHREAD_MUTEX_INITIALIZER;
int	RunScheduler(void)
{
	while(TRUE)
	{
		if(is_empty(&readyQueue))
		{
			// printf("sleeping..\n");
			sleep(TIMESLICE);
		}
		else
		{
			pthread_mutex_lock(&readyQueueMutex);
			currentTcb = pop(&readyQueue);
			if (!currentTcb->pExitCode)
				append_left(&readyQueue, currentTcb);
			currentTcb->bRunnable =FALSE;
			if (!readyQueue.cnt && waitQueue.cnt)
				append_left(&readyQueue, pop(&waitQueue));
			__ContextSwitch(currentTcb, readyQueue.bottom->data);
			pthread_mutex_unlock(&readyQueueMutex);
			sleep(TIMESLICE);
		}
	}
}

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread)
{
	pthread_kill(pCurThread->tid, SIGUSR1);
	__thread_to_run(pNewThread);
}

