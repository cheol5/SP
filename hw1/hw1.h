#ifndef __HW1_H__

#define __HW1_H__

#define MAX_STORAGE_SIZE (65500)
#define STORAGE_NAME "storage"

typedef enum __BlockState{
    FREE_BLOCK = 0,
    ALLOC_BLOCK = 1
} BlockState;


typedef struct __Block {
    int blockOffset;
    int blockState;
    int sizeHead;
    int sizeTail;
    int keySize;
    int dataSize;
} Block;


extern void Init(void);
extern int InsertData(char* key, int keySize, char* pBuf, int bufSize);
extern int GetDataByKey(char* key, int keySize, char* pBuf, int bufSize);
extern int RemoveDataByKey(char* key, int keySize);
extern void InitStorage(void);
extern int GetBlocks(Block* pBuf, int bufSize);


#endif