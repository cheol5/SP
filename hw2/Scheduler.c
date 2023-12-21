#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "signal.h"
#include "deque.h"

Thread *currentTcb = 0;

int	RunScheduler(void)
{
	while(TRUE)
	{
		if(is_empty(&readyQueue))
		{
			printf("sleeping..\n");
			sleep(TIMESLICE);
		}
		else
		{
			// __thread_to_run(currentTcb);
			currentTcb = pop(&readyQueue);
			append_left(&readyQueue, currentTcb);
			currentTcb->bRunnable =FALSE;
			__ContextSwitch(currentTcb, readyQueue.bottom->data);
			sleep(TIMESLICE);
		}
	}
}

// time slice만큼 동작이 끝난 쓰레드를 ready상태로 만들어주는 함수
void __thread_to_ready(int signo) {
    Thread *pTh = currentTcb;
    pthread_mutex_lock(&(pTh->readyMutex));
    while (pTh->bRunnable == FALSE)
	{
		printf("%d:다시 잠든다\n", (int)pTh->tid);
        pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
	}
    pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_to_run(Thread* pTh)
{
	pthread_mutex_lock(&(pTh->readyMutex));
	pTh->bRunnable = TRUE;
	pthread_cond_signal(&(pTh->readyCond));
	pthread_mutex_unlock(&(pTh->readyMutex));
}

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread)
{
	pthread_kill(pCurThread->tid, SIGUSR1);
	__thread_to_run(pNewThread);
}

