#include "hw1.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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



#define MAX_BLOCK_NUM      (1024)

int PrintBlocks(void)
{

    Block pBuf[MAX_BLOCK_NUM];
    int numBlocks = 0;

    printf("Blocks: ");
    numBlocks = GetBlocks(pBuf, MAX_BLOCK_NUM);
    for(int i = 0; i < numBlocks; i++)
    {
        printf("[%d %d %d %d %d %d], ",
            pBuf[i].blockOffset, pBuf[i].blockState,
            pBuf[i].sizeHead, pBuf[i].sizeTail,
            pBuf[i].keySize, pBuf[i].dataSize);
    }
    printf("\n");
    return 0;

}

void testcase1(void)
{
    int pKeySize, pBufSize, pTempSize;
    char* pKey = NULL;
    char* pBuf = NULL;
    char* pTemp = NULL;
    int i, j, c;

    printf("=======TestCase1=======\n");

    for(i = 0; i < 10; ++i) {
        for(j = 0; j < 10; ++j) {
            pKeySize = keySize[i % KEY_SIZE] + 2;
            pBufSize = dataSize[j % DATA_SIZE] + 2;

            pKey = (char*)malloc(pKeySize);
            pBuf = (char*)malloc(pBufSize);

            sprintf(pKey, "%s%d%d", key[i % KEY_SIZE], i, j);
            sprintf(pBuf, "%s%d%d", data[j % DATA_SIZE], i, j);

            if(-1 == InsertData(pKey, pKeySize, pBuf, pBufSize)) {
                printf("TestCase 1: Fail\n");
                return;
            }

            free(pKey);
            free(pBuf);
        }
    }
    PrintBlocks();

    for(i = 9; i >= 0; --i) {
        for(j = 0; j < 10; ++j) {
            pKeySize = keySize[i % KEY_SIZE] + 2;
            pBufSize = dataSize[j % DATA_SIZE] + 2;
            pTempSize = dataSize[j % DATA_SIZE] + 2;

            pKey = (char*)malloc(pKeySize);
            pBuf = (char*)malloc(pBufSize);
            pTemp = (char*)malloc(pTempSize);

            sprintf(pKey, "%s%d%d", key[i % KEY_SIZE], i, j);
            sprintf(pBuf, "%s%d%d", data[j % DATA_SIZE], i, j);

            GetDataByKey(pKey, pKeySize, pTemp, pTempSize);

            for(c = 0;c < pTempSize; ++c)
            {
                if (pBuf[c] != pTemp[c])
                {
                    printf("TestCase 1: Fail\n");
                    return ;
                }
            }

            free(pKey);
            free(pBuf);
            free(pTemp);
        }
    }
    PrintBlocks();

    printf("TestCase 1: Finish\n");
}


void testcase2(void)
{
    int PRIME_NUM[6] = {2, 3, 5, 7, 11, 13};
    int isRemoved[100] = {0};
    int pKeySize, pBufSize;
    char* pKey = NULL;
    char* pBuf = NULL;
    int i, j, k;

    printf("=======TestCase2=======\n");

    for(k = 5; k >= 0; --k) {
        for(i = 0; i < 10; ++i) {
            for(j = 0; j < 10; ++j) {

                int idx = i * 10 + j;
                if(idx % PRIME_NUM[k] != 0 ) continue;

                pKeySize = keySize[i % KEY_SIZE] + 2;
                pBufSize = dataSize[j % DATA_SIZE] + 2;

                pKey = (char*)malloc(pKeySize);
                pBuf = (char*)malloc(pBufSize);

                sprintf(pKey, "%s%d%d", key[i % KEY_SIZE], i, j);
                sprintf(pBuf, "%s%d%d", data[j % DATA_SIZE], i, j);

                int ret = RemoveDataByKey(pKey, pKeySize);

                if(isRemoved[idx] == 0 && ret == -1) {
                     printf("TestCase 2: Fail\n");
                     return;
                }
                else if(isRemoved[idx] == 1 && ret != -1) {
                     printf("TestCase 2: Fail\n");
                     return;
                }

                if(-1 != GetDataByKey(pKey, pKeySize, pBuf, pBufSize)) {
                     printf("TestCase 2: Fail\n");
                     return;
                }

                isRemoved[idx] = 1;

                free(pKey);
                free(pBuf);
            }
        }
    }
    PrintBlocks();

    for(i = 0; i < 10; ++i) {
        for(j = 0; j < 10; ++j) {

            int idx = i * 10 + j;
            if(isRemoved[idx] != 0) continue;

            pKeySize = keySize[i % KEY_SIZE] + 2;
            pBufSize = dataSize[j % DATA_SIZE] + 2;

            pKey = (char*)malloc(pKeySize);
            pBuf = (char*)malloc(pBufSize);

            sprintf(pKey, "%s%d%d", key[i % KEY_SIZE], i, j);
            sprintf(pBuf, "%s%d%d", data[j % DATA_SIZE], i, j);

            int ret = RemoveDataByKey(pKey, pKeySize);

            if(ret == -1) {
                 printf("TestCase 2: Fail\n");
                 return;
            }

            if(-1 != GetDataByKey(pKey, pKeySize, pBuf, pBufSize)) {
                 printf("TestCase 2: Fail\n");
                 return;
            }

            isRemoved[idx] = 1;

            free(pKey);
            free(pBuf);
        }
    }

    PrintBlocks();
    printf("TestCase 2: Finish\n");
}


void testcase3(void)
{
    int pKeySize, pBufSize, pTempSize;
    char* pKey = NULL;
    char* pBuf = NULL;
    char* pTemp = NULL;
    int i, j, c;
    int ret;

    printf("=======TestCase3=======\n");

    for(i = 2; i >= 0; --i) {
        for(j = 0; j < 10; ++j) {
            pKeySize = keySize[i] + 1;
            pBufSize = dataSize[i];

            pKey = (char*)malloc(pKeySize);
            pBuf = (char*)malloc(pBufSize);

            sprintf(pKey, "%s%d", key[i], j);
            sprintf(pBuf, "%s", data[i]);

            if(-1 == InsertData(pKey, pKeySize, pBuf, pBufSize)) {
                printf("TestCase 3: Fail\n");
                return;
            }

            free(pKey);
            free(pBuf);
        }

        for(j = 0; j < 10; j += 2) {
            pKeySize = keySize[i] + 1;
            pKey = (char*)malloc(pKeySize);
            sprintf(pKey, "%s%d", key[i], j);

            if(-1 == RemoveDataByKey(pKey, pKeySize)) {
                printf("TestCase 3: Fail\n");
                return;
            }

            free(pKey);
        }
    }
    PrintBlocks();

    for(i = 2; i >= 0; --i) {
        for(j = 1; j < 10; j += 2) {
            pKeySize = keySize[i] + 1;
            pBufSize = dataSize[i];
            pTempSize = dataSize[i];

            pKey = (char*)malloc(pKeySize);
            pBuf = (char*)malloc(pBufSize);
            pTemp = (char*)malloc(pTempSize);

            sprintf(pKey, "%s%d", key[i], j);
            sprintf(pBuf, "%s", data[i]);

            GetDataByKey(pKey, pKeySize, pTemp, pTempSize);

            for(c = 0;c < pTempSize; ++c)
            {
                if (pBuf[c] != pTemp[c])
                {
                    printf("TestCase 3: Fail\n");
                    return ;
                }
            }

            if(-1 == RemoveDataByKey(pKey, pKeySize)) {
                printf("TestCase 3: Fail\n");
                return;
            }

            free(pKey);
            free(pBuf);
            free(pTemp);
        }
    }
    PrintBlocks();

    for(i = 0; i < 3; ++i) {
        for(j = 0; j < (10 >> i); ++j) {
            pKeySize = keySize[i] + 1;
            pBufSize = dataSize[i];

            pKey = (char*)malloc(pKeySize);
            pBuf = (char*)malloc(pBufSize);

            sprintf(pKey, "%s%d", key[i], j);
            sprintf(pBuf, "%s", data[i]);

            if(-1 == InsertData(pKey, pKeySize, pBuf, pBufSize)) {
                printf("TestCase 3: Fail\n");
                return;
            }

            free(pKey);
            free(pBuf);
        }

        for(j = 1; j < (10 >> i) - 1; ++j) {
            pKeySize = keySize[i] + 1;
            pKey = (char*)malloc(pKeySize);
            sprintf(pKey, "%s%d", key[i], j);

            if(-1 == RemoveDataByKey(pKey, pKeySize)) {
                printf("TestCase 3: Fail\n");
                return;
            }

            free(pKey);
        }
    }
    PrintBlocks();


    for(i = 0; i < 3; ++i) {
        pKeySize = keySize[i] + 1;
        pBufSize = dataSize[i];
        pTempSize = dataSize[i];

        pKey = (char*)malloc(pKeySize);
        pBuf = (char*)malloc(pBufSize);
        pTemp = (char*)malloc(pTempSize);

        sprintf(pKey, "%s%d", key[i], 0);
        sprintf(pBuf, "%s", data[i]);

        GetDataByKey(pKey, pKeySize, pTemp, pTempSize);

        for(c = 0;c < pTempSize; ++c)
        {
            if (pBuf[c] != pTemp[c])
            {
                printf("TestCase 3: Fail\n");
                return ;
            }
        }

        if(-1 == RemoveDataByKey(pKey, pKeySize)) {
            printf("TestCase 3: Fail\n");
            return;
        }

        free(pKey);
        free(pBuf);
        free(pTemp);
    }

    for(i = 0; i < 3; ++i) {
        pKeySize = keySize[i] + 1;
        pBufSize = dataSize[i];
        pTempSize = dataSize[i];

        pKey = (char*)malloc(pKeySize);
        pBuf = (char*)malloc(pBufSize);
        pTemp = (char*)malloc(pTempSize);

        sprintf(pKey, "%s%d", key[i], (10 >> i) - 1);
        sprintf(pBuf, "%s", data[i]);

        GetDataByKey(pKey, pKeySize, pTemp, pTempSize);

        for(c = 0;c < pTempSize; ++c)
        {
            if (pBuf[c] != pTemp[c])
            {
                printf("TestCase 3: Fail\n");
                return ;
            }
        }

        if(-1 == RemoveDataByKey(pKey, pKeySize)) {
            printf("TestCase 3: Fail\n");
            return;
        }

        free(pKey);
        free(pBuf);
        free(pTemp);
    }
    PrintBlocks();

    printf("TestCase 3: Finish\n");
}



int main(int argc, char* argv[])
{
    int tcNum;

    if(argc!= 2)
    {
        printf("Input TestCase Number!\n");
        return 0;
    }

    tcNum = atoi(argv[1]);
    Init();
    switch(tcNum)
    {
        case 1:
            InitStorage();
            testcase1();
            break;
        case 2:
            testcase2();
            break;
        case 3:
            InitStorage();
            testcase3();
            break;
    }

    return 0;
}


