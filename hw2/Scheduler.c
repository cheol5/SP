#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "signal.h"


int		RunScheduler( void )
{
}

void __thread_to_ready2(Thread *pTh)
{
	pthread_mutex_lock(&(pTh->readyMutex));
	while (pTh->bRunnable == FALSE)
		pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
	pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_to_ready(int signo) {
    Thread *pTh = readyQueue.pop(readyQueue); // deque에서 찾아서 head의 thread를 부여한다. 
    pthread_mutex_lock(&(pTh->readyMutex));
    while (pTh->bRunnable == FALSE)
        pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
    pthread_mutex_unlock(&(pTh->readyMutex));

    // deque처리
	readyQueue.append_left(&readyQueue, pTh);
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
	pthread_kill(pCurThread, SIGUSR1);
	__thread_to_run(pNewThread);
}

