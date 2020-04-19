#include "simulator.h"

/************************************************************************************************
Description:
			This function initializes the instruction queue.
************************************************************************************************/
state* initializeInstructionQueue(state instructionQueue[])
{
	for (int i = 0; i < SIZE_OF_INSTRUCTION_QUEUE; i++) {
		instructionQueue[i].pc = -1;
		strcpy(instructionQueue[i].unit, "none");
		instructionQueue[i].cycleIssued = -1;
		instructionQueue[i].cycleReadOperands = -1;
		instructionQueue[i].cycleExecuteEnd = -1;
		instructionQueue[i].cycleWriteResult = -1;
	}
}

int findIndexForNewInst(state instructionQueue[])
{
	int indexForNewInst = -1;
	for (int i = 0; i < SIZE_OF_INSTRUCTION_QUEUE; i++)
	{
		if (instructionQueue[i].pc == -1 && instructionQueue[i].instruction != -2) //empty and not dirty
			return i;
	}


}

/************************************************************************************************
Description:
			This function inserts an instruction into the instruction queue.
************************************************************************************************/
int insertInstructionIntoQueue(state instructionQueue[], int instruction, int insPc)
{
	for (int i = 0; i < SIZE_OF_INSTRUCTION_QUEUE; i++)
	{
		if (instructionQueue[i].pc == -1)
		{
			strcpy(instructionQueue[i].unit, "none");
			instructionQueue[i].instruction = instruction;
			instructionQueue[i].pc = insPc;
			instructionQueue[i].cycleIssued = -1;
			instructionQueue[i].cycleReadOperands = -1;
			instructionQueue[i].cycleExecuteEnd = -1;
			instructionQueue[i].cycleWriteResult = -1;
			return EXIT_SUCCESS; //there was enough space in queue 
		}
	}
	return EXIT_ERROR; //there was not enough space in queue 
}


/************************************************************************************************
Description:
			This function prevents holes in the instruction queue.
************************************************************************************************/
void moveElementsOfinstructionQueue(state instructionQueue[], int deletedIndex)
{
	int lastQueueIndex = SIZE_OF_INSTRUCTION_QUEUE - 1;
	for (int i = deletedIndex; i < SIZE_OF_INSTRUCTION_QUEUE; i++)
	{
		strcpy(instructionQueue[i].unit, instructionQueue[i + 1].unit);
		instructionQueue[i].pc = instructionQueue[i + 1].pc;
		instructionQueue[i].instruction = instructionQueue[i + 1].instruction;
		instructionQueue[i].cycleWriteResult = instructionQueue[i + 1].cycleWriteResult;
		instructionQueue[i].cycleReadOperands = instructionQueue[i + 1].cycleReadOperands;
		instructionQueue[i].cycleIssued = instructionQueue[i + 1].cycleIssued;
		instructionQueue[i].cycleExecuteEnd = instructionQueue[i + 1].cycleExecuteEnd;
	}
	instructionQueue[lastQueueIndex].pc = -1;
	instructionQueue[lastQueueIndex].instruction = -1;
	strcpy(instructionQueue[lastQueueIndex].unit, "none");
	instructionQueue[lastQueueIndex].cycleIssued = -1;
	instructionQueue[lastQueueIndex].cycleReadOperands = -1;
	instructionQueue[lastQueueIndex].cycleExecuteEnd = -1;
	instructionQueue[lastQueueIndex].cycleWriteResult = -1;
}


/************************************************************************************************
Description:
			This function deletes an instruction from the instruction queue.
************************************************************************************************/
int deleteInstructionFromQueue(state instructionQueue[], int insPc)
{
	for (int i = 0; i < SIZE_OF_INSTRUCTION_QUEUE; i++)
	{
		if (instructionQueue[i].pc == insPc)
		{
			moveElementsOfinstructionQueue(instructionQueue, i);
			return EXIT_SUCCESS; //the instruction was deleted from queue 
		}
	}
	return EXIT_ERROR; //the insruction was not in queue 
}


/************************************************************************************************
Description:
			This function checks if a previous instruction is already trying to write the memory address
			which is represented by the immediate value.
************************************************************************************************/
bool checkForWAWHazard(state instructionQueue[], int currInsOpcode, int currInsImmediate)
{
	int immediate, destination, opcode, src0, src1;
	if (currInsOpcode == 0 || currInsOpcode == 1)
	{
		for (int i = 0; i < SIZE_OF_INSTRUCTION_QUEUE; i++)
		{
			defineRegisters(instructionQueue[i].instruction, &opcode, &destination, &src0, &src1, &immediate);
			if (currInsOpcode == 1 && opcode == 0 && immediate == currInsImmediate) //there's already a LD instruction that
				return WAW_HAZARD_DETECTED;                                        //read from the same address
			if (opcode == 1 && immediate == currInsImmediate)                     //there's already a ST instruction that 
				return WAW_HAZARD_DETECTED;                                      //writes to the same address
		}
	}
	return NO_WAW_HAZARD;
}


/************************************************************************************************
Description:
			Copies all fields of ins in instructionQueue into traceinst array.
************************************************************************************************/
void insertInsIntoTraceinst(state instructionQueue[], int ind, state traceinst[])
{
	int indexInStates = instructionQueue[ind].pc;
	strcpy(traceinst[indexInStates].unit, instructionQueue[ind].unit);
	traceinst[indexInStates].pc = indexInStates;
	traceinst[indexInStates].instruction = instructionQueue[ind].instruction;
	traceinst[indexInStates].cycleIssued = instructionQueue[ind].cycleIssued;
	traceinst[indexInStates].cycleReadOperands = instructionQueue[ind].cycleReadOperands;
	traceinst[indexInStates].cycleExecuteEnd = instructionQueue[ind].cycleExecuteEnd;
	traceinst[indexInStates].cycleWriteResult = instructionQueue[ind].cycleWriteResult;
}


/************************************************************************************************
Description:
			This function checks if the trace unit is busy.
************************************************************************************************/
bool isBusyTraceUnit(unitStatus* unitStatusArray, state instructionQueue[], char traceUnit[], int indOfTraceUnit,
	int cycle, int *traceUnitIndInQueue)
{
	for (int i = 0; i < SIZE_OF_INSTRUCTION_QUEUE; i++)
	{
		if (!strcmp(instructionQueue[i].unit, traceUnit))//unit had found
			*traceUnitIndInQueue = i;
	}
	if (*traceUnitIndInQueue == -1)
		return false;
	if ((unitStatusArray[indOfTraceUnit].instInd != -1) && (instructionQueue[*traceUnitIndInQueue].cycleIssued != cycle)) //trace unit busy
		return true;
	return false;
}

/************************************************************************************************
Description:
			This function checks if the previous instruction already finished the issue level.
************************************************************************************************/
int checkIfPrevInsIssued(state instructionQueue[], int indInInstructionQueue)
{
	int prevInsPc = instructionQueue[indInInstructionQueue].pc - 1;
	if (indInInstructionQueue == 0)
		return EXIT_SUCCESS;
	if ((instructionQueue[indInInstructionQueue - 1].pc) == prevInsPc) //previous instruction had not finished yet
	{
		if (instructionQueue[indInInstructionQueue - 1].cycleIssued != -1) //previous instruction was issued
			return EXIT_SUCCESS;
		else
			return EXIT_ERROR;
	}
	return EXIT_SUCCESS; //previous instruction had already finished 

}

/************************************************************************************************
Description:
			This function run the fetch level of the pipeline.
************************************************************************************************/
int* fetchInstruction(int haltFlag, int *numOfInsInQueue, state instructionQueue[], int *mainMemoryArrayPtr, int *insPcPtr)
{
	int currInsImmediate, destination, opcode, src0, src1;

	if (!haltFlag && (*numOfInsInQueue) < SIZE_OF_INSTRUCTION_QUEUE) { //there is still space for another ins in queue 
		defineRegisters(*mainMemoryArrayPtr, &opcode, &destination, &src0, &src1, &currInsImmediate);
		if (checkForWAWHazard(instructionQueue, opcode, currInsImmediate) == NO_WAW_HAZARD) {
			if (insertInstructionIntoQueue(instructionQueue, *mainMemoryArrayPtr, *insPcPtr))//fetch successed
			{
				mainMemoryArrayPtr++; //update main memory pointer
				(*insPcPtr)++; //update insructions pc
				(*numOfInsInQueue)++;
				return mainMemoryArrayPtr;
			}
		}
	}
	return mainMemoryArrayPtr;
}

/************************************************************************************************
Description:
			This function run the issue level of the pipeline.
************************************************************************************************/
void issueLevel(int cycle, state instructionQueue[], unitStatus* unitStatusArray, int totalNumOfUnits,
	registerStatus* registersArray)
{
	int ind = 0;
	char unitName[MAX_UNIT_NAME_LENGTH];

	for (ind = 0; ind < SIZE_OF_INSTRUCTION_QUEUE; ind++)  //loop for all the instructions in queue
	{
		if (cycle > 0 && instructionQueue[ind].pc != -1) //a fetched instruction
		{
			if (!strcmp(instructionQueue[ind].unit, "none") && checkIfPrevInsIssued(instructionQueue, ind)) //not issued before and prev instruction had already issued 
			{
				if (insertInstructionIntoUnitStatus(unitStatusArray, instructionQueue[ind].instruction,
					instructionQueue[ind].pc, totalNumOfUnits, registersArray, unitName))
				{
					strcpy(instructionQueue[ind].unit, unitName); //issue one instruction
					instructionQueue[ind].cycleIssued = cycle;
					return;
				}
				else                            // we couldn't issue the instruction- all others need to wait
				{
					return;
				}
			}
		}
	}
}

/************************************************************************************************
Description:
			This function run the read operands level of the pipeline.
************************************************************************************************/
void readOperandsLevel(int cycle, state instructionQueue[], unitStatus* unitStatusArray, int totalNumOfUnits, char unitName[], registerStatus* registersArray)
{
	int ind = 0, indOfUnit = 0, RjNo = 0, RkNo = 0;

	for (ind = 0; ind < SIZE_OF_INSTRUCTION_QUEUE; ind++)  //loop for all the instructions in queue
	{
		if ((strcmp(instructionQueue[ind].unit, "none") != 0) && (instructionQueue[ind].cycleIssued != cycle)
			&& (instructionQueue[ind].cycleReadOperands == -1))// an issued instruction that had not read operands yet
		{
			indOfUnit = findInsInUnitStatusArray(unitStatusArray, totalNumOfUnits, instructionQueue[ind].pc);
			updateQRjndQRk(unitStatusArray, indOfUnit, registersArray);
			RjNo = strcmp(unitStatusArray[indOfUnit].Rj, "Yes");
			RkNo = strcmp(unitStatusArray[indOfUnit].Rk, "Yes");
			if (!RjNo && !RkNo)                                          //read operand
			{
				unitStatusArray[indOfUnit].FjVal = registersArray[unitStatusArray[indOfUnit].Fj].registerVal;
				unitStatusArray[indOfUnit].FkVal = registersArray[unitStatusArray[indOfUnit].Fk].registerVal;
				instructionQueue[ind].cycleReadOperands = cycle;
			}
		}
		indOfUnit = findInsInUnitStatusArray(unitStatusArray, totalNumOfUnits, instructionQueue[ind].pc);
		RjNo = strcmp(unitStatusArray[indOfUnit].Rj, "Yes");
		RkNo = strcmp(unitStatusArray[indOfUnit].Rk, "Yes");
		if ((instructionQueue[ind].cycleReadOperands == (cycle - 1)) && (!RjNo && !RkNo)) //read operand one cycle ago
		{
			strcpy(unitStatusArray[indOfUnit].Rj, "No");
			strcpy(unitStatusArray[indOfUnit].Rk, "No");
		}
	}
}

/************************************************************************************************
Description:
			This function run the execute level of the pipeline.
************************************************************************************************/
void executeLevel(int cycle, state instructionQueue[], unitStatus* unitStatusArray, int totalNumOfUnits,
	functionalUnit functionalUnitTypes[])
{
	int ind = 0, indOfUnit = 0, unitDelay = 0;
	for (ind = 0; ind < SIZE_OF_INSTRUCTION_QUEUE; ind++)  //loop for all the instructions in queue
	{
		indOfUnit = findInsInUnitStatusArray(unitStatusArray, totalNumOfUnits, instructionQueue[ind].pc);
		if (indOfUnit != NO_INDEX_FOUND) {
			unitDelay = functionalUnitTypes[unitStatusArray[indOfUnit].opcode].unitDelay;
			if ((instructionQueue[ind].cycleReadOperands != -1) && (cycle == instructionQueue[ind].cycleReadOperands + unitDelay - 1)) //execute
				instructionQueue[ind].cycleExecuteEnd = cycle;
		}
	}
}

/************************************************************************************************
Description:
			This function run the write result level of the pipeline.
************************************************************************************************/
void writeResultLevel(int cycle, int mainMemoryArray[], state instructionQueue[], unitStatus* unitStatusArray, int totalNumOfUnits,
	registerStatus* registersArray, int *completedInsCounter, int *numOfInsInQueue, state traceinst[])
{
	int ind = 0, indOfUnit = 0, Fi = 0, isSTOpcode = 0;

	for (ind = 0; ind < SIZE_OF_INSTRUCTION_QUEUE; ind++)  //loop for all the instructions in queue
	{
		if ((instructionQueue[ind].cycleExecuteEnd != -1) && (instructionQueue[ind].cycleExecuteEnd != cycle)) //already was executed
		{
			indOfUnit = findInsInUnitStatusArray(unitStatusArray, totalNumOfUnits, instructionQueue[ind].pc);
			Fi = unitStatusArray[indOfUnit].Fi;
			isSTOpcode = unitStatusArray[indOfUnit].opcode;
			if (!strcmp(unitStatusArray[indOfUnit].unitName, registersArray[Fi].Qi) || (isSTOpcode == 1))
			{
				execInstruction(instructionQueue[ind].instruction, unitStatusArray[indOfUnit].opcode, unitStatusArray[indOfUnit].Fi,
					unitStatusArray[indOfUnit].Fj, unitStatusArray[indOfUnit].Fk, mainMemoryArray, registersArray); //write result
				instructionQueue[ind].cycleWriteResult = cycle;
				insertInsIntoTraceinst(instructionQueue, ind, traceinst);
				if (isSTOpcode != 1)
					strcpy(registersArray[Fi].Qi, "none");  //change the QI in registers
				deleteInstructionFromQueue(instructionQueue, instructionQueue[ind].pc);//delete ins from queue
				ind--; //check again this index because maybe the next unit has also ended an its now on this index
				(*numOfInsInQueue)--;
				deleteInstructionFromUnitStatus(unitStatusArray, indOfUnit);//delete ins from unitStatusArray
				(*completedInsCounter)++;
			}
		}
	}
}

/************************************************************************************************
Description:
			This function writes to trace instruction file.
************************************************************************************************/
void writeToTraceInstFile(char *pathOftraceInstFile, state *traceinstArray, int totalNumOfIns)
{
	FILE *fPtrWriteTraceInst = NULL;
	fPtrWriteTraceInst = openFile(pathOftraceInstFile, fPtrWriteTraceInst, WRITE_SELECT);
	for (int i = 0; i < totalNumOfIns; i++)
	{
		fprintf(fPtrWriteTraceInst, "%08x %d %s %d %d %d %d\n", traceinstArray[i].instruction, traceinstArray[i].pc,
			traceinstArray[i].unit, traceinstArray[i].cycleIssued, traceinstArray[i].cycleReadOperands,
			traceinstArray[i].cycleExecuteEnd, traceinstArray[i].cycleWriteResult);
	}
	fclose(fPtrWriteTraceInst);//close memout file
}


/************************************************************************************************
Description:
			This function helps writing a line to the trace unit file.
************************************************************************************************/
void writeToTraceUnitHelper(unitStatus* unitStatusArray, state instructionQueue[], int indOfTraceUnit, int cycle,
	FILE *fPtrWriteTraceUnit, int traceUnitIndInQueue)
{
	if (strstr(unitStatusArray[indOfTraceUnit].unitName, "LD") != NULL) {
		fprintf(fPtrWriteTraceUnit, "%d %s ", cycle, unitStatusArray[indOfTraceUnit].unitName);
		fprintf(fPtrWriteTraceUnit, "F%d - ", unitStatusArray[indOfTraceUnit].Fi);
		fprintf(fPtrWriteTraceUnit, "- - ");
		fprintf(fPtrWriteTraceUnit, "- - ");
		fprintf(fPtrWriteTraceUnit, "-\n");
	}
	else if (strstr(unitStatusArray[indOfTraceUnit].unitName, "ST") != NULL)
	{
		fprintf(fPtrWriteTraceUnit, "%d %s ", cycle, unitStatusArray[indOfTraceUnit].unitName);
		fprintf(fPtrWriteTraceUnit, "- - ", unitStatusArray[indOfTraceUnit].Fi, unitStatusArray[indOfTraceUnit].Fj);
		fprintf(fPtrWriteTraceUnit, "F%d - ", unitStatusArray[indOfTraceUnit].Fk, unitStatusArray[indOfTraceUnit].Qj);
		fprintf(fPtrWriteTraceUnit, "%s - ", unitStatusArray[indOfTraceUnit].Qk, unitStatusArray[indOfTraceUnit].Rj);
		fprintf(fPtrWriteTraceUnit, "%s\n", unitStatusArray[indOfTraceUnit].Rk);
	}
	else
	{
		fprintf(fPtrWriteTraceUnit, "%d %s ", cycle, unitStatusArray[indOfTraceUnit].unitName);
		fprintf(fPtrWriteTraceUnit, "F%d F%d ", unitStatusArray[indOfTraceUnit].Fi, unitStatusArray[indOfTraceUnit].Fj);
		fprintf(fPtrWriteTraceUnit, "F%d %s ", unitStatusArray[indOfTraceUnit].Fk, unitStatusArray[indOfTraceUnit].Qj);
		fprintf(fPtrWriteTraceUnit, "%s %s ", unitStatusArray[indOfTraceUnit].Qk, unitStatusArray[indOfTraceUnit].Rj);
		fprintf(fPtrWriteTraceUnit, "%s\n", unitStatusArray[indOfTraceUnit].Rk);
	}
}

/************************************************************************************************
Description:
			This function writes a line to the trace unit file.
************************************************************************************************/
void writeToTraceUnit(unitStatus* unitStatusArray, state instructionQueue[], int indOfTraceUnit, int cycle,
	FILE *fPtrWriteTraceUnit, int traceUnitIndInQueue)
{
	writeToTraceUnitHelper(unitStatusArray, instructionQueue, indOfTraceUnit, cycle, fPtrWriteTraceUnit, traceUnitIndInQueue);

	if (instructionQueue[traceUnitIndInQueue].cycleExecuteEnd != -1)
	{
		writeToTraceUnitHelper(unitStatusArray, instructionQueue, indOfTraceUnit, cycle + 1,
			fPtrWriteTraceUnit, traceUnitIndInQueue);
	}
}


/************************************************************************************************
Description:
			This function is responsible for the proper passage of the pipeline instructions.
************************************************************************************************/
void instructionRoutine(int mainMemoryArray[], int totalNumOfUnits, int totalNumOfIns, state instructionQueue[],
	unitStatus* unitStatusArray, registerStatus* registersArray, functionalUnit functionalUnitTypes[], state traceinst[],
	char *pathOfTraceUnitFile, char traceUnit[])
{
	FILE *fPtrWriteTraceUnit = NULL;
	bool haltFlag = false;
	char unitName[MAX_UNIT_NAME_LENGTH];
	int *mainMemoryArrayPtr = NULL, cycle = 0, insPc = 0, *insPcPtr = &insPc, completedInsCounter = 0;
	int currInsOpcode = 0, numOfInsInQueue = 0, *completedInsCounterPtr, indOfTraceUnit = 0, traceUnitIndInQueue = -1;

	completedInsCounterPtr = &completedInsCounter;
	mainMemoryArrayPtr = mainMemoryArray;
	fPtrWriteTraceUnit = openFile(pathOfTraceUnitFile, fPtrWriteTraceUnit, WRITE_SELECT);
	indOfTraceUnit = findIndOfTraceUnit(unitStatusArray, totalNumOfUnits, traceUnit);
	while (true)
	{
		currInsOpcode = (int)(*mainMemoryArrayPtr & 0x0f000000);
		currInsOpcode >>= 24;
		if (currInsOpcode == 6) //check if the instruction is HALT
			haltFlag = true;
		mainMemoryArrayPtr = fetchInstruction(haltFlag, &numOfInsInQueue, instructionQueue, mainMemoryArrayPtr, insPcPtr);
		issueLevel(cycle, instructionQueue, unitStatusArray, totalNumOfUnits, registersArray);
		readOperandsLevel(cycle, instructionQueue, unitStatusArray, totalNumOfUnits, unitName, registersArray);
		executeLevel(cycle, instructionQueue, unitStatusArray, totalNumOfUnits, functionalUnitTypes);
		writeResultLevel(cycle, mainMemoryArray, instructionQueue, unitStatusArray, totalNumOfUnits, registersArray, completedInsCounterPtr,
			&numOfInsInQueue, traceinst);
		if (isBusyTraceUnit(unitStatusArray, instructionQueue, traceUnit, indOfTraceUnit, cycle, &traceUnitIndInQueue))
			writeToTraceUnit(unitStatusArray, instructionQueue, indOfTraceUnit, cycle, fPtrWriteTraceUnit, traceUnitIndInQueue);
		cycle++;
		if (haltFlag && (completedInsCounter == totalNumOfIns))//loop break condition
			break;
	}
	fclose(fPtrWriteTraceUnit);//close trace unit file
}



int main(int argc, char *argv[])
{
	int mainMemoryArray[SIZE_OF_MEMORY] = { 0 }, totalNumOfUnits = 0, totalNumOfIns = 0;
	char traceUnit[MAX_UNIT_NAME_LENGTH] = "";
	unitStatus* unitStatusArray = NULL;
	functionalUnit functionalUnitTypes[NUM_OF_UNIT_TYPES];
	state instructionQueue[SIZE_OF_INSTRUCTION_QUEUE], *traceinst = NULL;
	registerStatus registersArray[NUM_OF_REGISTERS];

	initializeInstructionQueue(instructionQueue);
	unitStatusArray = buildUnitsStatusArray(argv[1], functionalUnitTypes, &totalNumOfUnits, traceUnit);
	updateMainMemoryArrayFromFile(argv[2], mainMemoryArray); //create the memory array
	totalNumOfIns = getNumOfInsInMemory(mainMemoryArray);
	traceinst = (state*)malloc(totalNumOfIns * sizeof(state));
	initializeRegs(registersArray);
	instructionRoutine(mainMemoryArray, totalNumOfUnits, totalNumOfIns, instructionQueue, unitStatusArray,
		registersArray, functionalUnitTypes, traceinst, argv[6], traceUnit);
	writeToRegoutFile(argv[4], registersArray);
	writeToMemoutFile(argv[3], mainMemoryArray);
	writeToTraceInstFile(argv[5], traceinst, totalNumOfIns);
	free(unitStatusArray);
	free(traceinst);
}