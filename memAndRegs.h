#ifndef MEM_AND_REGS_H
#define MEM_AND_REGS_H
#include "workingWithFile.h"
#define MAX_CHARS_IN_MEMIN_LINE 10
#define NUM_OF_REGISTERS 16
#define SIZE_OF_MEMORY 4096

/************************************************************************************************
Description:
			This struct contains all the data of the registers- index, value and Qi (same as in scoreboard
			algorithm).
************************************************************************************************/
typedef struct registerStatus
{
	int registerInd;
	float registerVal;
	char Qi[MAX_UNIT_NAME_LENGTH];
} registerStatus;

int* updateMainMemoryArrayFromFile(char *pathOfMemInFile, int mainMemoryArray[]);
void initializeRegs(registerStatus* registersArray);
int getNumOfInsInMemory(int mainMemoryArray);
void writeToRegoutFile(char *pathOfRegoutFile, registerStatus* registersArray);
void writeToMemoutFile(char *pathOfRegoutFile, int mainMemoryArray[]);
#endif