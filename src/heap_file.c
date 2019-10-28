#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf.h"
#include "heap_file.h"


#define CALL_BF(call)       \
{                           \
  BF_ErrorCode code = call; \
  if (code != BF_OK) {      \
    BF_PrintError(code);    \
    return HP_ERROR;        \
  }                         \
}

HP_ErrorCode HP_Init() {
  //insert code here
  return HP_OK;
}

HP_ErrorCode HP_CreateFile(const char *filename) {
  //insert code here
  int fD;
  char *data;
  BF_Block *mBlock;
  BF_Block_Init(&mBlock);

  CALL_BF(BF_CreateFile(filename));
  CALL_BF(BF_OpenFile(filename, &fD));
  CALL_BF(BF_AllocateBlock(fD, mBlock));
  data = BF_Block_GetData(mBlock);
  memset(data, '\0', 512);
  memcpy(data, "HP", 2);                              // HP == Heap File

  BF_Block_SetDirty(mBlock);
  CALL_BF(BF_UnpinBlock(mBlock));
  BF_Block_Destroy(&mBlock);

  return HP_OK;
}

HP_ErrorCode HP_OpenFile(const char *fileName, int *fileDesc){
  //insert code here
  char *data;
  char code[2];
  BF_Block *mBlock;
  BF_Block_Init(&mBlock);

  CALL_BF(BF_OpenFile(fileName, fileDesc))
  CALL_BF(BF_GetBlock(*fileDesc, 0, mBlock));
  data = BF_Block_GetData(mBlock);
  memcpy(code, data, 2);
  if(strcmp(code, "HP")) {
    return HP_ERROR;
  }

  CALL_BF(BF_UnpinBlock(mBlock));
  BF_Block_Destroy(&mBlock);
  return HP_OK;;
}

HP_ErrorCode HP_CloseFile(int fileDesc) {
  //insert code here
  CALL_BF(BF_CloseFile(fileDesc));
  return HP_OK;
}

HP_ErrorCode HP_InsertEntry(int fileDesc, Record record) {
  //insert code here
  int block_num;
  char records_num;
  char *data;
  BF_Block *mBlock;
  BF_Block_Init(&mBlock);

  CALL_BF(BF_GetBlockCounter(fileDesc, &block_num));
  if(block_num == 1) {
    CALL_BF(BF_AllocateBlock(fileDesc, mBlock));
    data = BF_Block_GetData(mBlock);
    memset(data, '\0', 512);
    memset(data, 1, 1);                                   // STORING NUM OF RECORDS ON THE BLOCK
    memset(data+1, record.id & 0xFF, 1);
    memset(data+2, (record.id >> 8) & 0xFF, 1);
    memset(data+3, (record.id >> 16) & 0xFF, 1);
    memset(data+4, (record.id >> 24) & 0xFF, 1);
    memcpy(data + 1 + 4, record.name, 15);
    memcpy(data + 1 + 4 + 15, record.surname, 20);
    memcpy(data + 1 + 4 + 15 + 20, record.city, 20);
  }
  else {
    CALL_BF(BF_GetBlock(fileDesc, block_num-1, mBlock));
    data = BF_Block_GetData(mBlock);
    records_num = *data;
    if(records_num == 8) {
      CALL_BF(BF_UnpinBlock(mBlock));
      CALL_BF(BF_AllocateBlock(fileDesc, mBlock));
      data = BF_Block_GetData(mBlock);
      memset(data, '\0', 512);
      memset(data, 1, 1);
      memset(data + 1, record.id & 0xFF, 1);
      memset(data + 2, (record.id >> 8) & 0xFF, 1);
      memset(data + 3, (record.id >> 16) & 0xFF, 1);
      memset(data + 4, (record.id >> 24) & 0xFF, 1);
      memcpy(data + 1 + 4, record.name, 15);
      memcpy(data + 1 + 4 + 15, record.surname, 20);
      memcpy(data + 1 + 4 + 15 + 20, record.city, 20);
    }
    else {
      memset(data, records_num+1, 1);
      memset(data + 1 + records_num*59, record.id & 0xFF, 1);
      memset(data + 2 + records_num*59, (record.id >> 8) & 0xFF, 1);
      memset(data + 3 + records_num*59, (record.id >> 16) & 0xFF, 1);
      memset(data + 4 + records_num*59, (record.id >> 24) & 0xFF, 1);
      memcpy(data + 1 + records_num*59 + 4, record.name, 15);
      memcpy(data + 1 + records_num*59 + 4 + 15, record.surname, 20);
      memcpy(data + 1 + records_num*59 + 4 + 15 + 20, record.city, 20);
    }
  }

  BF_Block_SetDirty(mBlock);
  CALL_BF(BF_UnpinBlock(mBlock));
  BF_Block_Destroy(&mBlock);

  return HP_OK;
}

void HP_PrintRecord(char *data, int i) {
  int id = *(int*)(data + i*59 + 1);
  char name[15], sur[20], city[20];

  memcpy(name, data + i*59 + 5, 15);
  memcpy(sur, data + i*59 + 20, 20);
  memcpy(city, data + i*59 + 40, 20);

  printf("  ID == %d\n", id);
  printf("  NAME == %s\n", name);
  printf("  SURNAME == %s\n", sur);
  printf("  CITY == %s\n", city);
}

HP_ErrorCode HP_PrintAllEntries(int fileDesc, char *attrName, void* value) {
  //insert code here
  int block_num;
  char records_num;
  char *data;
  BF_Block *mBlock;
  BF_Block_Init(&mBlock);

  CALL_BF(BF_GetBlockCounter(fileDesc, &block_num));

  for(int i=1; i<block_num; i++) {
    CALL_BF(BF_GetBlock(fileDesc, i, mBlock));
    data = BF_Block_GetData(mBlock);
    records_num = *data;
    for(int j=0; j<records_num; j++) {
      int id;
      char name[15], sur[20], city[20];

      if(value == NULL) {
        printf("-----------------------\n In Block number : %d\n Record number : %d\n\n", i, j);
        HP_PrintRecord(data, j);
        printf("-----------------------\n");
      }

      else if(!strcmp(attrName, "name")) {
        memcpy(name, data + j*59 + 5, 15);
        if(!strcmp(name, (char*)value)) {
          printf("-----------------------\n In Block number : %d\n Record number : %d\n\n", i, j);
          HP_PrintRecord(data, j);
          printf("-----------------------\n");
        }
      }
      else if(!strcmp(attrName, "surname")) {
        memcpy(sur, data + j*59 + 20, 20);
        if(!strcmp(sur, (char*)value)) {
          printf("-----------------------\n In Block number : %d\n Record number : %d\n\n", i, j);
          HP_PrintRecord(data, j);
          printf("-----------------------\n");
        }
      }
      else if(!strcmp(attrName, "city")) {
        memcpy(city, data + j*59 + 40, 20);
        if(!strcmp(city, (char*)value)) {
          printf("-----------------------\n In Block number : %d\n Record number : %d\n\n", i, j);
          HP_PrintRecord(data, j);
          printf("-----------------------\n");
        }
      }
    }
    CALL_BF(BF_UnpinBlock(mBlock));
  }
  printf("\n");
  BF_Block_Destroy(&mBlock);
  return HP_OK;
}

HP_ErrorCode HP_GetEntry(int fileDesc, int rowId, Record *record) {
  //insert code here
  int block_num;
  char records_num;
  char *data;
  BF_Block *mBlock;
  BF_Block_Init(&mBlock);


  block_num = (rowId-1)/8 + 1;
  CALL_BF(BF_GetBlock(fileDesc, block_num, mBlock));
  data = BF_Block_GetData(mBlock);
  records_num = (rowId-1)%8;
  record->id = *(int*)(data + records_num*59 + 1);
  memcpy(record->name, data + records_num*59 + 5, 15);
  memcpy(record->surname, data + records_num*59 + 20, 20);
  memcpy(record->city, data + records_num*59 + 40, 20);

  CALL_BF(BF_UnpinBlock(mBlock));
  BF_Block_Destroy(&mBlock);
  return HP_OK;
}

HP_ErrorCode HP_PrintUniversal(int fileDesc) {
  int block_num;
  char records_num;
  char *data;
  BF_Block *mBlock;
  BF_Block_Init(&mBlock);

  CALL_BF(BF_GetBlockCounter(fileDesc, &block_num));
  for(int i=1; i<block_num; i++) {
    printf("\nBLock Num = %d\n", i);
    CALL_BF(BF_GetBlock(fileDesc, i, mBlock));
    data = BF_Block_GetData(mBlock);
    records_num = data[0];
    for(int j=0; j<records_num; j++) {
      printf("Record num = %d\n", j);
      HP_PrintRecord(data, j);
    }
    CALL_BF(BF_UnpinBlock(mBlock));
  }
  BF_Block_Destroy(&mBlock);
  return HP_OK;
}