
#include "my.h"
#include "hw1.h"

// !! Write를 하면 자동으로 current File offset이 write다음 바이트로 변경됨.

int 		fd;
BlockState	State;

int GetBlocks(Block* pBuf, int bufSize)
{
	int				i;
	char			arr[HEAD];
	int				offset = 0;
	unsigned short	blcokSize;

	i = 0;
	memset(pBuf, 0, sizeof(Block) * bufSize);
	offset = lseek(fd, 0, SEEK_SET); //first in fit
	while (i < bufSize && offset < MAX_STORAGE_SIZE) 
	{
		if (!(read(fd, arr, HEAD)))
			break ;
		memcpy(&pBuf[i].blockState, &arr[0], 1);
		memcpy(&pBuf[i].blockOffset, &offset, 4);
		memcpy(&pBuf[i].keySize, &arr[3], 1);
		memcpy(&pBuf[i].dataSize, &arr[4], 1);
		pBuf[i].sizeHead = HEAD;
		pBuf[i].sizeTail = TAIL;
		memcpy(&blcokSize, &arr[1], 2);
		offset = lseek(fd, blcokSize - HEAD, SEEK_CUR);
		i++;
	}
	return (i);
}

void Init(void)
{
	fd = 0;
}

void InitStorage(void)
{
	unsigned short	blockSize = MAX_STORAGE_SIZE;
	char			blockState = FREE_BLOCK;

	// WRONLY를 빼먹으면 파일 작성이 되지 않는다. O_TRUNC : 한번 만들어진 파일은 재생성 되지 않음. 
	if ((fd = open(STORAGE_NAME, O_CREAT|O_RDWR|O_TRUNC, 0777)) < 0)
	{
		perror("open");
		exit (1);
	}
	if(truncate(STORAGE_NAME, MAX_STORAGE_SIZE))
	{
		perror("truncate error is occurd\n");
		exit(1);
	}
	write(fd, &blockState, 1);
	write(fd, &blockSize, 2);
	lseek(fd, -2, SEEK_END);
	write(fd, &blockSize, 2);
}

// 남은 블럭을 null, 표식자, 크기로 채워넣는다. 
static void	leftBlock(unsigned short leftBlockSize)
{
	char	*leftBlock;
	char	blockState = FREE_BLOCK;

	leftBlock = (char *)malloc(leftBlockSize);
	memset(leftBlock, 0, leftBlockSize);
	memcpy(leftBlock, &blockState, 1);
	memcpy(&leftBlock[1], &leftBlockSize, 2);
	memcpy(&leftBlock[leftBlockSize - 2], &leftBlockSize, 2);
	write(fd, leftBlock, leftBlockSize);
	free(leftBlock);
}

//동적할당 후 (mem)cpy이후 head정보를 보고 삽입 위치 결정.
int InsertData(char* key, int keySize, char* pBuf, int bufSize)
{
	char			*arr;
	unsigned short	arrSize;
	unsigned short	blockSize;
	char			buf[BUFFER_SIZE];
	int				sum = 0;
	int				offset;

	arrSize = keySize + bufSize + HEAD + TAIL; 
	arr = (char *)malloc(sizeof(char) * (arrSize));
	if (!arr)
		return (0);
	memset(arr, 0, arrSize);
	arr[0] = ALLOC_BLOCK;
	memcpy(&arr[1], &arrSize, 2);
	memcpy(&arr[1 + 2], &keySize, 1);
	memcpy(&arr[1 + 2 + 1], &bufSize, 1);
	memcpy(&arr[5], key, keySize);
	memcpy(&arr[5 + keySize], pBuf, bufSize);
	memcpy(&arr[arrSize - 2], &arrSize, 2);
	offset = lseek(fd, 0, SEEK_SET); // first in fit.
	while (read(fd, buf, BUFFER_SIZE))
	{
		memcpy(&blockSize, &buf[1], 2);
		if (buf[0] == ALLOC_BLOCK || blockSize < arrSize)
		{
			//EOF의 범위를 넘어가면 exit!!
			if (offset + blockSize -BUFFER_SIZE + arrSize > MAX_STORAGE_SIZE)
			{
				free(arr);
				perror("There's no storage");
				exit(1);
			}
			offset = lseek(fd, blockSize - BUFFER_SIZE, SEEK_CUR);
		}
		else
			break;
	}
	lseek(fd, -BUFFER_SIZE, SEEK_CUR);
	write(fd, arr, arrSize);
	// Assumption 항상 null로 채워져있다고 가정. 삭제할 때 null처리하기!
	if (blockSize > arrSize + HEAD + TAIL)
		leftBlock(blockSize - arrSize);
	free(arr);
	return (1);
}

//성공 시 return값 : 읽은 data의 크기. 실패 시 -1
int GetDataByKey(char* key, int keySize, char* pBuf, int bufSize)
{
	char			buf[HEAD];
	char			*arrOfKey;
	unsigned short	blockSize;
	int				offset;

	offset = lseek(fd, 0, SEEK_SET); //first in fit
	while (read(fd, buf, HEAD) && offset < MAX_STORAGE_SIZE)
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
				if (!arrOfKey)
					return (0);
				read(fd, arrOfKey, buf[4]);
				memcpy(pBuf, arrOfKey, buf[4]);
				free(arrOfKey);
				return ((int)buf[4]);
			}
			free(arrOfKey);
			offset = lseek(fd, blockSize - HEAD - keySize, SEEK_CUR);
		}
		else
			offset = lseek(fd, blockSize - HEAD, SEEK_CUR);
	}
	return (-1);
}

static void	afterRemove(char *currntBuf, int keySize, int currentBlockSize, int offset)
{
	char			frontBuf[HEAD] = {0};
	char			tailBuf[HEAD] = {0};
	char			bufSize[2] = {0};
	char			*newBlock;
	unsigned short	frontBlockSize = 0;
	unsigned short	tailBlockSize = 0;
	unsigned short	sum = currentBlockSize;

	// current file offset position is currentBlock offset + HEAD + keySize
	if (offset - HEAD - keySize - 2 > 0) // current block이 첫 블록이 아니면
	{
		offset = lseek(fd, -(keySize + HEAD) - 2, SEEK_CUR);
		read(fd, bufSize, 2);
		memcpy(&frontBlockSize, bufSize, 2);
		lseek(fd, -frontBlockSize, SEEK_CUR);
		read(fd, frontBuf, HEAD); // front buf stored.
		offset = lseek(fd, -HEAD + frontBlockSize + currentBlockSize, SEEK_CUR);
	}
	else
		offset = lseek(fd, currentBlockSize - HEAD - keySize, SEEK_CUR);
	read(fd, tailBuf, HEAD);
	memcpy(&tailBlockSize, &tailBuf[1], 2);
	offset = lseek(fd, -HEAD - frontBlockSize - currentBlockSize, SEEK_CUR);
	if (frontBuf[0] == FREE_BLOCK && offset >= 0)
		sum += frontBlockSize;
	else
		offset = lseek(fd, frontBlockSize, SEEK_CUR);
	if (tailBuf[0] == FREE_BLOCK)
		sum += tailBlockSize;
	leftBlock(sum);
}

// 삭제 대상이 없으면 -1을 반환. 있으면 1반환
int RemoveDataByKey(char* key, int keySize)
{
	char			buf[HEAD];
	char			*arrOfKey;
	char			*arr;
	unsigned short	blockSize;
	int				offset;

	fd = open(STORAGE_NAME, O_RDWR, 0777);
	offset = lseek(fd, 0, SEEK_SET); //first in fit
	while (read(fd, buf, HEAD) && offset < MAX_STORAGE_SIZE)
	{
		memcpy(&blockSize, &buf[1], 2);
		if (keySize == buf[3])
		{
			arrOfKey = (char *)malloc(sizeof(char) * keySize + 1);
			arrOfKey[keySize] = 0;
			read(fd, arrOfKey, keySize);
			if(!strcmp(key, arrOfKey))
			{
				afterRemove(buf, keySize, blockSize, offset);
				free(arrOfKey);
				return (1);
			}
			free(arrOfKey);
			offset = lseek(fd, blockSize - HEAD - keySize, SEEK_CUR);
		}
		else
			offset = lseek(fd, blockSize - HEAD, SEEK_CUR);
	}
	return (-1);
}
