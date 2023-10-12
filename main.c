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
int	fd;
int	main(void)
{
	char arr[100] = {0};
	int nbytes = 0;
    int j = 0;
    Init();
	InitStorage();
	for (int i = 0; i < 4; i++)
		InsertData(key[i], keySize[i], data[i], dataSize[i]);
	nbytes = GetDataByKey(key[j], keySize[j], arr, 100);
    printf("key : %s, data : %s, dataSize : %d\n", key[j], arr, nbytes);
    return 0;
}

