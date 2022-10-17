#ifndef FILEOUT_H
#define FILEOUT_H

#include <fstream>

class CFileOut
{
public:
  CFileOut(void);
  ~CFileOut(void);
public:
  int WriteFile(char* fileName,uint8_t *dt,int dtLen);
  int ReadFile(char* fileName,uint8_t *dt,int rLen);
};

#endif // FILEOUT_H
