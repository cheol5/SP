# include "dmddo.h"

int myNum = 10;

void signalTest(int signo)
{
	printf("calling signal\n");
}

int main()
{
	signal(SIGINT, signalTest);
	printf("%d\n", myNum);
	foo();
	while(1)
	{
		signalTest(0);
		sleep(2);
	}
}