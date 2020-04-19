#include "workingWithFile.h"


/************************************************************************************************
Description:
			This function open a file and reports in any case of error.
*************************************************************************************************/
FILE *openFile(char *pathOfFile, FILE *filePtr, int RWSelect)
{
	if (!RWSelect)
		filePtr = fopen(pathOfFile, "r");
	if (RWSelect)
		filePtr = fopen(pathOfFile, "w");
	if (filePtr == NULL) {
		printf("An error occurred while opening file\n");
		exit(EXIT_ERROR);
	}
	return filePtr;
}