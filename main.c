# include "hw1.h"

int main(int ac, char *av[])
{
	int	fd;
	if (fd = open("test.txt", O_RDONLY) < 0)
	{
		perror("open");
		exit (1);
	}
}