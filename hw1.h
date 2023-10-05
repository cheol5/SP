#ifndef __HW1_H__

#define __HW1_H__

#define MAX_STORAGE_SIZE (66536)
#define STORAGE_NAME "storage"

typedef enum __BlockState{
    FREE_BLOCK = 0,
    ALLOC_BLOCK = 1
} BlockState;

extern void Init(void);
extern int InsertData(char* key, int keySize, char* pBuf, int bufSize);
extern int getDataByKey(char* key, int keySize, char* pBuf, int bufSize);
extern int RemoveDataByKey(char* key, int keySize);
extern void InitStorage(void);


#endif