#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "signal.h"
#include "deque.c"

int		RunScheduler( void )
{
}

// time slice만큼 동작이 끝난 쓰레드를 테일에 붙여주는 함수.
void __thread_to_ready(int signo) {
    Thread *pTh = pop(&readyQueue);
    pthread_mutex_lock(&(pTh->readyMutex));
    while (pTh->bRunnable == FALSE)
        pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
    pthread_mutex_unlock(&(pTh->readyMutex));

    // tail에 다시 붙여준다.
	append_left(&readyQueue, pTh);
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

