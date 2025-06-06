#ifndef FILE_H
#define FILE_H

#include <stdio.h>

#include "dac/string.h"

#define FileMode_Read "r"
#define FileMode_Write "w"

typedef struct {
  FILE* stdFile;
  String path;
} File;

File* File_Open(String, String);

size_t File_GetLength(File*);

String File_ReadText(File*);

#endif