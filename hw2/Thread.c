#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "deque.h"

Thread*     ReadyQHead = NULL;
Thread*     ReadyQTail = NULL;

Thread*     WaitQHead = NULL;
Thread*     WaitQTail = NULL;

void tcbBlockInit(Thread *block)
{
	
}

int 	thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	//tcb 생성해야함.
	Thread tcbBlock;
	tcbBlockInit(&tcbBlock);
	pthread_create(thread, 0, start_routine, arg);
	return 0;
}


int 	thread_join(thread_t thread, void **retval)
{

}


int 	thread_suspend(thread_t tid)
{

}


int	thread_resume(thread_t tid)
{

}




thread_t	thread_self()
{
	return pthread_self();
}	
