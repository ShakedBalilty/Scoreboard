#ifndef WORKING_WITH_FILE_H
#define WORKING_WITH_FILE_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "commonCons.h"
#define READ_SELECT 0
#define WRITE_SELECT 1

FILE *openFile(char *pathOfFile, FILE *filePtr, int RWSelect);
#endif