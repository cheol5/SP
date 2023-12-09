# include "dmddo.h"

int main()
{
	if (fork() == 0)
		execl("/usr/bin/cp", "cp", "foo", "bar", 0);
	wait(0);
	printf("cpy!! \n");
	exit(0);
}