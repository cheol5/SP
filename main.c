# include "my.h"
# include "hw1.h"

// caution : header에서 extern 으로 선언 하고 여기서 따로 선언해서 사용해야 전역변수로 사용가능.
int	fd;
int	main(void)
{
	char arr[3] = {0};
	Init();
	InitStorage();
	//InsertData()
	return 0;
}

