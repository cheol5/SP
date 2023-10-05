
#include "my.h"
#include "hw1.h"

static int	strlcpy(char *dst, char *src, size_t n)
{
	size_t	i;
	size_t	len;

	len = strlen(src);
	if (!n)
		return (len);
	while(src[i] && i < n - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (len);
}

static int	getLength(int n)
{
	int	len = 0;

	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char *itoa(int n)
{
	int		i = getLength(n);
	char	*arr;

	arr = (char *)malloc(sizeof(char) * (i + 1));
	if (!arr)
		return (0);
	arr[i--] = 0;
	arr[0] = '0';
	while (n)
	{
		arr[i--] = n % 10 + '0';
		n /= 10;
	}
	if (n < 0)
		arr[0] = '-';
	return(arr);
}

static void itoaHW(char *itoaArr, int num)
{
	int	i = 1;

	itoaArr[0] = '0';
	while (num)
	{
		itoaArr[i--] = num % 10 + '0';
		num /= 10;
	}
}

// void	initFile(int fd)
// {
// 	const char	*initData = "A66536";
// 	int			size;

// 	size = strlen(initData);

// 	write(fd, initData, size);
// 	lseek(fd, -(size - 1), SEEK_END);
// 	write(fd, initData + 1, size - 1);
// }

// !! Write를 하면 자동으로 current File offset이 write다음 바이트로 변경됨.
void Init(void)
{
	int		i = 0;
	char	itoaArr[3] = {0};
	// WRONLY를 빼먹으면 파일 작성이 되지 않는다. 한번 만들어진 파일은 재생성 되지 않음. 
	if ((fd = open(STORAGE_NAME, O_CREAT|O_RDWR|O_TRUNC, 0755)) < 0)
	{
		perror("open");
		exit (1);
	}
	if(truncate(STORAGE_NAME, MAX_STORAGE_SIZE))
	{
		perror("truncate error is occurd\n");
		exit(1);
	}
	//block 초기화를 해야하는데 음.. 큰 블럭으로 보고 할 것인지를 결정해야해.. 
	while (1)
	{
		write(fd, "A99", 3); //write
		lseek(fd, 99 - 3 - 2, SEEK_CUR);
		write(fd, "99", 2);
		i += 99;
		if (i + 99 > MAX_STORAGE_SIZE)
		{
			i = MAX_STORAGE_SIZE - i;
			if (i < HEAD_TAIL_SIZE)
				return ;
			itoaHW(itoaArr, i);
			write(fd, "A", 1);
			write(fd, itoaArr, strlen(itoaArr));
			lseek(fd, -2, SEEK_END);
			write(fd, itoaArr, 2);
			return ;
		}
	}
}





// caution : 만약 9이하의 블럭이라면  09 이렇게 저장되어야 한다. -> 해결 itoaHW
static char	*insertDataInit(char *key, char *pBuf, int arrSize)
{
	char	*arr;
	char	itoaArr[3] = {0};
	int		keySize;
	int		pbufSize;
	int		itoaSize;

	arr = (char *)malloc(sizeof(char) * arrSize);
	if (!arr)
		return (0);
	memset(arr, 0, arrSize); // I think that initializing with 0 ensure null charcater!!
	itoaHW(itoaArr, arrSize);
	keySize = strlen(key);
	pbufSize = strlen(pBuf);
	itoaSize = strlen(itoaArr);
	arr[0] = 'F';
	strlcpy(&arr[1], itoaArr, itoaSize + 1);
	strlcpy(&arr[1 + itoaSize], key, keySize + 1);
	strlcpy(&arr[1 + itoaSize + keySize], pBuf, pbufSize + 1);
	return (arr);
}

static int	afterInsert(int leftBlockSize)
{
	char	*leftArr;
	char	itoaArr[3] = {0};

	leftArr = (char *)malloc(sizeof(char) * leftBlockSize + 1);
	if (!leftArr)
		return (0);
	leftArr[0] = 'A';
	itoaHW(itoaArr, leftBlockSize);
	strlcpy(&leftArr[1], itoaArr, 3);
	return (1);
}

//caution : fd를 전역변수로 빼던지 여기서 다시 open and close해야함. -> 해결: 우선 헤더에 선언
//caution : insert이후의 과정이 필요하다 -> make afterInsert. 
//logic : 동적할당 후 cpy. 이후 head정보를 보고 삽입 위치 결정.
int InsertData(char* key, int keySize, char* pBuf, int bufSize)
{
	char	*arr;
	char	buf[BUFFER_SIZE];
	int		arrSize;
	int		nbytes;

	arrSize = keySize + bufSize + HEAD_TAIL_SIZE + 1; // 1:Ensure null character. 
	arr = insertDataInit(key, pBuf, arrSize); // cpy operation.
	lseek(fd, 0, SEEK_SET);
	nbytes = read(fd, buf, BUFFER_SIZE - 1); // Use nbytes that notify EOF.
	buf[BUFFER_SIZE - 1] = 0;
	// Search Insert position.
	while ((buf[0] == 'F' || atoi(&buf[1]) < arrSize) && nbytes)
	{
		lseek(fd, atoi(&buf[1]) - 1, SEEK_CUR);
		nbytes = read(fd, buf, BUFFER_SIZE);
	}
	if (!nbytes)
	{
		if (atoi(&buf[1]) - arrSize + 1)
		write(fd, arr, arrSize - 1);
		//afterInsert()
		free(arr);
	}
	else
	{
		perror("File is fulled\n");
		exit(1);
	}
	return (arrSize - 1);
}

// strcmp 이용
int getDataByKey(char* key, int keySize, char* pBuf, int bufSize)
{
	char	buf[6];
	int		nbytes;
	char	*arrOfKey;

	nbytes = read(fd, buf, 6);
	// while (nbytes && )
	// {
	// 	keySize = atoi(buf[3]);
	// 	arrOfKey = (char *)malloc(sizeof(char) * keySize + 1);
	// 	arrOfKey[keySize] = 0;
	// 	read(fd, arrOfKey, keySize)
	// }
	return (0);
}

// int RemoveDataByKey(char* key, int keySize)
// {

// }

// void InitStorage(void)
// {

// }

