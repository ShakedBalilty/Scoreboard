#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <stdio.h>
#include <stdbool.h>
#include "functionalUnit.h"
#define SIZE_OF_INSTRUCTION_QUEUE 16
#define NO_WAW_HAZARD 0
#define WAW_HAZARD_DETECTED -1


/************************************************************************************************
Description:
			This struct contains all the data as in states array of scoreboard algorithm (Instruction status).
************************************************************************************************/
typedef struct State
{
	int instruction;
	int pc;
	char unit[MAX_UNIT_NAME_LENGTH];
	int cycleIssued;
	int cycleReadOperands;
	int cycleExecuteEnd;
	int cycleWriteResult;
} state;

state* initializeInstructionQueue(state instructionQueue[]);
int insertInstructionIntoQueue(state instructionQueue[], int instruction, int insPc);
void moveElementsOfinstructionQueue(state instructionQueue[], int deletedIndex);
int deleteInstructionFromQueue(state instructionQueue[], int insPc);
void insertInsIntoTraceinst(state instructionQueue[], int ind, state traceinst[]);
bool checkForWAWHazard(state instructionQueue[], int currInsOpcode, int currInsImmediate);
bool isBusyTraceUnit(unitStatus* unitStatusArray, state instructionQueue[], char traceUnit[], int indOfTraceUnit, int cycle,
	int *traceUnitIndInQueue);
int checkIfPrevInsIssued(state instructionQueue[], int indInInstructionQueue);
int* fetchInstruction(int haltFlag, int *numOfInsInQueue, state instructionQueue[], int *mainMemoryArrayPtr, int *insPcPtr);
void issueLevel(int cycle, state instructionQueue[], unitStatus* unitStatusArray, int totalNumOfUnits, registerStatus* registersArray);
void readOperandsLevel(int cycle, state instructionQueue[], unitStatus* unitStatusArray, int totalNumOfUnits,
	char unitName[], registerStatus* registersArray);
void executeLevel(int cycle, state instructionQueue[], unitStatus* unitStatusArray, int totalNumOfUnits,
	functionalUnit functionalUnitTypes[]);
void writeResultLevel(int cycle, int mainMemoryArray[], state instructionQueue[], unitStatus* unitStatusArray, int totalNumOfUnits,
	registerStatus* registersArray, int *completedInsCounter, int *numOfInsInQueue, state traceinst[]);
void writeToTraceInstFile(char *pathOftraceInstFile, state *traceinstArray, int totalNumOfIns);
void writeToTraceUnit(unitStatus* unitStatusArray, state instructionQueue[], int indOfTraceUnit, int cycle,
	FILE *fPtrWriteTraceUnit, int traceUnitIndInQueue);
void instructionRoutine(int mainMemoryArray[], int totalNumOfUnits, int totalNumOfIns, state instructionQueue[],
	unitStatus* unitStatusArray, registerStatus* registersArray, functionalUnit functionalUnitTypes[], state traceinst[],
	char *pathOfTraceUnitFile, char traceUnit[]);
#endif 