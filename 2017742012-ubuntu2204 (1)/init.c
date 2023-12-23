#include "Init.h"
#include "deque.h"
#include "Thread.h"
#include "signal.h"
#include "Scheduler.h"

void Init(void){
	signal(SIGUSR1, __thread_to_ready);
}