# include "my.h"
# include "hw1.h"

int main()
{
	char buf[2] = {0};
	int	a = 0;
	int fd = open(STORAGE_NAME, O_RDWR, 0777);

	lseek(fd, -2, SEEK_END);
	read(fd, buf, 2);
	memcpy(&a, buf, 2);
	printf("tailSize : %d\n", a);
	lseek(fd, -a + 1, SEEK_CUR);
	read(fd, buf, 2);
	memcpy(&a, buf, 2);
	printf("HEADSize : %d\n", a);
	
}