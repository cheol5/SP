# include "my.h"
# include "hw1.h"

#define KEY_SIZE 4
#define DATA_SIZE 5

int keySize[KEY_SIZE]   = { 6, 5, 4, 2 };
int dataSize[DATA_SIZE] = { 2, 19, 52, 15, 5 };

char* key[KEY_SIZE]   = { "system",
                          "token",
                          "card",
                          "ID" };

char* data[DATA_SIZE] = { "Hi",
                          "KwangwoonUniversity",
                          "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
                          "OperatingSystem",
                          "Apple" };

// caution : header에서 extern 으로 선언 하고 여기서 따로 선언해서 사용해야 전역변수로 사용가능.

int	main(void)
{
	char arr[200] = {0};
	int nbytes = 0;
    int j = 0;
    Block buf[100] = {0};

    Init();
	InitStorage();
	for (int i = 0; i < 4; i++)
		InsertData(key[i], keySize[i], data[i], dataSize[i]);
	nbytes = GetDataByKey(key[j], keySize[j], arr, 100);
    for (int j = 0; j < 4; j++)
    {
        memset(arr, 0, 100);
        nbytes = GetDataByKey(key[j], keySize[j], arr, 100);
        printf("key : %s, data : %s, dataSize : %d\n", key[j], arr, nbytes);
    }
    GetBlocks(buf, 100);
	for (int i = 0; i < 4; i++)
    {
        printf("buf[%d].blockState : %c\n", i, buf[i].blockState);
        printf("buf[%d].blockOffset : %d\n", i, buf[i].blockOffset);
        printf("buf[%d].keySize : %d\n", i, buf[i].keySize);
        printf("buf[%d].dataSize : %d\n", i, buf[i].dataSize);
        printf("buf[%d].sizeHead : %d\n", i, buf[i].sizeHead);
        printf("buf[%d].sizeTail : %d\n", i, buf[i].sizeTail);
    }
    // for (int i = 0; i<4; i++)
    //     RemoveDataByKey(key[i], keySize[i]);
    if (RemoveDataByKey(key[2], keySize[2]) == 1)
        printf("Remove %s\n", key[2]);
    GetBlocks(buf, 100);
	for (int i = 0; i < 4; i++)
    {
        printf("buf[%d].blockState : %c\n", i, buf[i].blockState);
        printf("buf[%d].blockOffset : %d\n", i, buf[i].blockOffset);
        printf("buf[%d].keySize : %d\n", i, buf[i].keySize);
        printf("buf[%d].dataSize : %d\n", i, buf[i].dataSize);
        printf("buf[%d].sizeHead : %d\n", i, buf[i].sizeHead);
        printf("buf[%d].sizeTail : %d\n", i, buf[i].sizeTail);
    }
    //RemoveDataByKey(key[1], keySize[1]);
    //RemoveDataByKey(key[3], keySize[3]);
    //if (RemoveDataByKey(key[0], keySize[0]) == 1)
    //    printf("Remove %s\n", key[0]);
    return 0;
}

