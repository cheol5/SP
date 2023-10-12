
#include "my.h"
#include "hw1.h"

// !! Write를 하면 자동으로 current File offset이 write다음 바이트로 변경됨.
void Init(void)
{
	fd = 0;
}

void InitStorage(void)
{
	unsigned short blockSize = MAX_STORAGE_SIZE - 1;
	char arr[]="This is the test\n";
	unsigned short check;
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
	write(fd, "A", 1);
	write(fd, &blockSize, 2);
	lseek(fd, -2, SEEK_END);
	write(fd, &blockSize, 2);
}

static int	leftBlock(unsigned short leftBlockSize)
{
	write(fd, "A", 1);
	write(fd, &leftBlockSize, 2);
	lseek(fd, leftBlockSize - BUFFER_SIZE - 2, SEEK_CUR);
	write(fd, &leftBlockSize, 2);
}

//동적할당 후 (mem)cpy이후 head정보를 보고 삽입 위치 결정.
int InsertData(char* key, int keySize, char* pBuf, int bufSize)
{
	char			*arr;
	unsigned short	arrSize;
	unsigned short	blockSize;
	char			buf[BUFFER_SIZE];

	arrSize = keySize + bufSize + HEAD + TAIL; 
	arr = (char *)malloc(sizeof(char) * (arrSize));
	if (!arr)
		return (0);
	memset(arr, 0, arrSize);
	arr[0] = 'F';
	memcpy(&arr[1], &arrSize, 2);
	memcpy(&arr[1 + 2], &keySize, 1);
	memcpy(&arr[1 + 2 + 1], &bufSize, 1);
	memcpy(&arr[5], key, keySize);
	memcpy(&arr[5 + keySize], pBuf, bufSize);
	lseek(fd, 0, SEEK_SET); // first in fit.
	while (read(fd, buf, BUFFER_SIZE)) // EOF이후에 lseek하게 되어 read하는 예외케이스 존재가능.
	{
		memcpy(blockSize, &buf[1], 2);
		if (buf[0] == 'F' || blockSize < arrSize)
		{
			if (SEEK_CUR + arrSize > MAX_STORAGE_SIZE)
			{
				free(arr);
				perror("There's no storage");
				exit(1);
			}
			lseek(fd, blockSize - BUFFER_SIZE, SEEK_CUR);
		}
		else
			break;
	}
	write(fd, arr, arrSize); // offset 더해짐 주의
	// Assumption 항상 null로 채워져있다고 가정. 삭제할 때 null처리하기!
	if (blockSize > arrSize + HEAD + TAIL)
		leftBlock(blockSize - arrSize);
	free(arr);
	return ;
}

int getDataByKey(char* key, int keySize, char* pBuf, int bufSize)
{
	char			buf[HEAD];
	char			*arrOfKey;
	unsigned short	blockSize;

	lseek(fd, 0, SEEK_CUR);
	while (read(fd, buf, HEAD))
	{
		memcpy(&blockSize, &buf[1], 2);
		if (keySize == buf[3])
		{
			arrOfKey = (char *)malloc(sizeof(char) * keySize + 1);
			arrOfKey[keySize] = 0;
			read(fd, arrOfKey, keySize);
			if(!strcmp(key, arrOfKey))
			{
				// search 성공
				free(arrOfKey);
				// dataSize만큼 할당 후 read하고 cpy.
				arrOfKey = (char *)malloc(sizeof(char) * buf[4]);
				read(fd, arrOfKey, buf[4]);
				memcpy(pBuf, arrOfKey, buf[4]);
				free(arrOfKey);
				lseek(fd, 0, SEEK_SET); // offset처음으로.
				return ;
			}
			free(arrOfKey);
			lseek(fd, blockSize - HEAD - keySize, SEEK_CUR);
		}
		else
			lseek(fd, blockSize - HEAD, SEEK_CUR);
	}
	return ;
}

int RemoveDataByKey(char* key, int keySize)
{
	char			buf[HEAD];
	char			*arrOfKey;
	unsigned short	blockSize;

	while (read(fd, buf, HEAD))
	{
		memcpy(&blockSize, &buf[1], 2);
		if (keySize == buf[3])
		{
			arrOfKey = (char *)malloc(sizeof(char) * keySize + 1);
			arrOfKey[keySize] = 0;
			read(fd, arrOfKey, keySize);
			if(!strcmp(key, arrOfKey)) // return 0이면 같음! 
			{
				// search!! 앞뒤블럭 탐색 후 합치기 드가자
				// 근데 하나의 함수에 너무 많은 기능이 있는 것은 좋지 않을 것 같기도 해서 고민중인데 일단은
				// remove만 작성하려고 함. 
				free(arrOfKey);
				arrOfKey = (char *)malloc(sizeof(char) * blockSize);
				read(fd, arrOfKey, buf[4]);
				free(arrOfKey);
				return 1;
			}
			free(arrOfKey);
			lseek(fd, blockSize - HEAD - keySize, SEEK_CUR);
		}
		else
			lseek(fd, blockSize - HEAD, SEEK_CUR);
	}
	return 1;
}

// void InitStorage(void)
// {

// }

