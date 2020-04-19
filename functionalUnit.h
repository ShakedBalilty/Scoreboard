#ifndef FUNCTIONAL_UNIT_H
#define FUNCTIONAL_UNIT_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include "Instructions.h"

#define NUM_OF_UNIT_TYPES 6
#define MAX_CHARS_IN_YES_OR_NO 4


/************************************************************************************************
Description:
			This struct contains all types of units and details about them - the delay time of each unit type and
			the number of units available. The order of all types is predetermined - {LW, SW, ADD, SUB, MUL, DIV}
************************************************************************************************/
typedef struct FunctionalUnit
{
	char unitType[4];
	int numberOfUnits;
	int unitDelay;
} functionalUnit;

/************************************************************************************************
Description:
			This struct contains all the data as in units array of scoreboard algorithm. The
			fields "FjVal" and "FkVal" are intended to save the registers' value after read operand level
************************************************************************************************/
typedef struct UnitStatus
{
	int instInd;
	int opcode;
	char unitName[MAX_UNIT_NAME_LENGTH];
	int Fi;
	int Fj;
	int FjVal;
	int Fk;
	int FkVal;
	char Qj[MAX_UNIT_NAME_LENGTH];
	char Qk[MAX_UNIT_NAME_LENGTH];
	char Rk[MAX_CHARS_IN_YES_OR_NO];
	char Rj[MAX_CHARS_IN_YES_OR_NO];
} unitStatus;


int getParameterFromString(char *string);
int getTraceUnit(char *lineInCfgFile, char traceUnit[]);
int addNumOfUnits(char *lineInCfgFile, functionalUnit functionalUnitTypes[]);
int addDelayOfUnit(char *lineInCfgFile, functionalUnit functionalUnitTypes[]);
void updateConfigurations(char *lineInCfgFile, functionalUnit functionalUnitTypes[], char traceUnit[]);
void getProgramCfgs(char *pathOfCfgFile, functionalUnit functionalUnitTypes[], char traceUnit[]);
void updateUnitNames(int functionalUnitNum, char* unitType, unitStatus *unitStatusArray, int indOfUnitType, int opcode);
unitStatus* buildUnitsStatusArray(char *pathOfCfgFile, functionalUnit functionalUnitTypes[], int *totalNumOfUnits,
	char traceUnit[]);
void deleteInstructionFromUnitStatus(unitStatus* unitStatusArray, int indOfUnit);
int findInsInUnitStatusArray(unitStatus* unitStatusArray, int totalNumOfUnits, int instructionInd);
int insertInstructionIntoUnitStatus(unitStatus* unitStatusArray, int memCurrIns, int insPc, int totalNumOfUnits, registerStatus *registersArray, char unitname[]);
void updateQRjndQRk(unitStatus* unitStatusArray, int indOfUnit, registerStatus* registersArray);
int findIndOfTraceUnit(unitStatus* unitStatusArray, int totalNumOfUnits, char traceUnit[]);
#endif