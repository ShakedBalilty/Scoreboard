#include "memAndRegs.h"

/************************************************************************************************
Description:
			This function create a new array of int numbers named mainMemoryArray, with the lines
			of memin text file.
************************************************************************************************/
int* updateMainMemoryArrayFromFile(char *pathOfMemInFile, int mainMemoryArray[])
{
	int i = 0;
	char temp[MAX_CHARS_IN_MEMIN_LINE];
	FILE *fPtrReadMemin = NULL;
	fPtrReadMemin = openFile(pathOfMemInFile, fPtrReadMemin, READ_SELECT);
	while (feof(fPtrReadMemin) == 0) //loop until the end of the file
	{
		fgets(temp, MAX_CHARS_IN_MEMIN_LINE, fPtrReadMemin); //get a line of the configutation file
		mainMemoryArray[i] = (int)strtol(temp, NULL, 16);
		i++;
		strcpy(temp, "00000000");
	}
	fclose(fPtrReadMemin);//close memin file
	return mainMemoryArray;
}


/************************************************************************************************
Description:
			This function checks how many instructions there are in main memory.
************************************************************************************************/
int getNumOfInsInMemory(int mainMemoryArray)
{
	int *mainMemoryArrayPtr = mainMemoryArray, currInsOpcode = 0, numOfIns = 0;

	currInsOpcode = (int)(*mainMemoryArrayPtr & 0x0f000000);
	currInsOpcode >>= 24;
	while (currInsOpcode != 6)
	{
		numOfIns++;
		mainMemoryArrayPtr++;
		currInsOpcode = (int)(*mainMemoryArrayPtr & 0x0f000000);
		currInsOpcode >>= 24;
	}
	return numOfIns;
}


/************************************************************************************************
Description:
			This function initialize the register array.
************************************************************************************************/
void initializeRegs(registerStatus* registersArray)
{
	for (int i = 0; i < NUM_OF_REGISTERS; i++)
	{
		registersArray[i].registerInd = i;
		registersArray[i].registerVal = i / 1.0;
		strcpy(registersArray[i].Qi, "none");
	}
}

/************************************************************************************************
Description:
			This function writes to the registers' file.
************************************************************************************************/
void writeToRegoutFile(char *pathOfRegoutFile, registerStatus* registersArray)
{
	FILE *fPtrWriteRegout = NULL;
	fPtrWriteRegout = openFile(pathOfRegoutFile, fPtrWriteRegout, WRITE_SELECT);
	for (int i = 0; i < NUM_OF_REGISTERS; i++)
		fprintf(fPtrWriteRegout, "%f\n", registersArray[i].registerVal);
	fclose(fPtrWriteRegout);//close Regout file
}


/************************************************************************************************
Description:
			This function writes to the memout' file.
************************************************************************************************/
void writeToMemoutFile(char *pathOfRegoutFile, int mainMemoryArray[])
{
	FILE *fPtrWriteRegout = NULL;
	fPtrWriteRegout = openFile(pathOfRegoutFile, fPtrWriteRegout, WRITE_SELECT);
	for (int i = 0; i < SIZE_OF_MEMORY; i++)
		fprintf(fPtrWriteRegout, "%08x\n", mainMemoryArray[i]);
	fclose(fPtrWriteRegout);//close memout file
}


