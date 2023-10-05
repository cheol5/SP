# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>

int main(void)
{
	unsigned short a = 65535;
	unsigned short b;
	unsigned short *c;
	char arr[3] = {0};
	int fd = open("hello", O_CREAT|O_RDWR, 0777);
	write(fd, &a, 2);
	lseek(fd, 0, SEEK_SET);
	read(fd, &b, 2);
	printf("b is %d\n", b);
	lseek(fd, 0, SEEK_SET);
	read(fd, c, 2);
	printf("c is %d\n", *c);
	
	close(fd);
}