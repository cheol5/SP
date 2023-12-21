# include "dmddo.h"

int myNum = 10;

void *signalTest(void *args)
{
	while (1){
	printf("child tid :%ld\n", pthread_self());
	sleep(2);
	}
}

void caller(int signo){
	printf("clclcl\n");
}

int main()
{
	signal(SIGUSR1, caller);
	printf("%d\n", myNum);
	foo();

	pthread_t tid;
	pthread_create(&tid, 0, signalTest, 0);
	while(1){
	pthread_kill(tid, SIGUSR1);
	sleep(2);
	}
	pthread_join(tid, 0);
	// printf("main : %ld\nand child :%ld", pthread_self(), tid);
}