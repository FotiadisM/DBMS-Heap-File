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
  memset(data, 0, 4);                              // 4 zeros == Heap File

  BF_Block_SetDirty(mBlock);
  CALL_BF(BF_UnpinBlock(mBlock));
  BF_Block_Destroy(&mBlock);

  return HP_OK;
}

HP_ErrorCode HP_OpenFile(const char *fileName, int *fileDesc){
  //insert code here
  CALL_BF(BF_OpenFile(fileName, fileDesc))
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
    memset(data, 1, 1);
    *(data+1) = record.id;
    memcpy(data + 1 + sizeof(record.id), record.name, sizeof(record.name));
    memcpy(data + 1 + sizeof(record.id) + sizeof(record.name), record.surname, sizeof(record.surname));
    memcpy(data + 1 + sizeof(record.id) + sizeof(record.name) + sizeof(record.surname), record.city, sizeof(record.city));
  }
  else {
    CALL_BF(BF_GetBlock(fileDesc, block_num-1, mBlock));
    data = BF_Block_GetData(mBlock);
    records_num = *data;
    if(records_num == 8) {
      CALL_BF(BF_UnpinBlock(mBlock));
      CALL_BF(BF_AllocateBlock(fileDesc, mBlock));
      data = BF_Block_GetData(mBlock);
      memset(data, 1, 1);
      *(data + 1) = record.id;
      memcpy(data + 1 + sizeof(record.id), record.name, sizeof(record.name));
      memcpy(data + 1 + sizeof(record.id) + sizeof(record.name), record.surname, sizeof(record.surname));
      memcpy(data + 1 + sizeof(record.id) + sizeof(record.name) + sizeof(record.surname), record.city, sizeof(record.city));
    }
    else {
      memset(data, records_num+1, 1);
      *(data + 1 + records_num*sizeof(Record)) = record.id;
      memcpy(data + 1 + records_num*sizeof(Record) + sizeof(record.id), record.name, sizeof(record.name));
      memcpy(data + 1 + records_num*sizeof(Record) + sizeof(record.id) + sizeof(record.name), record.surname, sizeof(record.surname));
      memcpy(data + 1 + records_num*sizeof(Record) + sizeof(record.id) + sizeof(record.name) + sizeof(record.surname), record.city, sizeof(record.city));
    }
  }

  BF_Block_SetDirty(mBlock);
  CALL_BF(BF_UnpinBlock(mBlock));
  BF_Block_Destroy(&mBlock);

  return HP_OK;
}

HP_ErrorCode HP_PrintAllEntries(int fileDesc, char *attrName, void* value) {
  //insert code here
  return HP_OK;
}

HP_ErrorCode HP_GetEntry(int fileDesc, int rowId, Record *record) {
  //insert code here
  return HP_OK;
}