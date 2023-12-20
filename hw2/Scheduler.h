#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "Thread.h"

int		RunScheduler( void );
void            	__ContextSwitch(Thread* pCurThread, Thread* pNewThread);
void __thread_to_run(Thread* pTh);
void __thread_to_ready(int signo);
void __thread_to_ready2(Thread *pTh);

#endif
