#include "FileOut.h"

CFileOut::CFileOut(void)
{
}

CFileOut::~CFileOut(void)
{
}

int CFileOut::WriteFile(char* fileName,uint8_t *dt,int dtLen)
{
  FILE *fp = NULL;
  fp = fopen(fileName,"wb");
  if(NULL == fp)
    return -1;

  fwrite(dt,sizeof(uint8_t),dtLen,fp);

  fclose(fp);
  return 0;
}

int CFileOut::ReadFile(char* fileName,uint8_t *dt,int rLen)
{
  FILE *fp = NULL;
  fp = fopen(fileName,"rb");
  if(NULL == fp)
    return -1;

  int len = fread(dt,sizeof(uint8_t),rLen,fp);

  fclose(fp);
  return len;
}
