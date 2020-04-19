#include "functionalUnit.h"


/************************************************************************************************
Description:
			This function gets the value of parameter from line in cfg file.
************************************************************************************************/

int getParameterFromString(char *string)
{
	int parameterVal = 0;
	char *parameterPtr = strstr(string, "=");
	parameterPtr = parameterPtr + 2;
	parameterVal = atoi(parameterPtr);
	return parameterVal;
}

/************************************************************************************************
Description:
			This function checks if the input line include the trace unit.
************************************************************************************************/
int getTraceUnit(char *lineInCfgFile, char traceUnit[])
{
	char *endOfName = NULL;
	if (strstr(lineInCfgFile, "trace_unit") != NULL)
	{
		endOfName = strstr(lineInCfgFile, "\n");
		if (endOfName != NULL)
			*endOfName = '\0';
		strcpy(traceUnit, lineInCfgFile + 13);

		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

/************************************************************************************************
Description:
			This function updates the number of units according to their type in the functionalUnitTypes array.
************************************************************************************************/
int addNumOfUnits(char *lineInCfgFile, functionalUnit functionalUnitTypes[])
{
	if (strstr(lineInCfgFile, "nr_units") != NULL)
	{
		if (strstr(lineInCfgFile, "ld") != NULL)
		{
			strcpy(functionalUnitTypes[0].unitType, "ld");
			functionalUnitTypes[0].numberOfUnits = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "st") != NULL)
		{
			strcpy(functionalUnitTypes[1].unitType, "st");
			functionalUnitTypes[1].numberOfUnits = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "add") != NULL)
		{
			strcpy(functionalUnitTypes[2].unitType, "add");
			functionalUnitTypes[2].numberOfUnits = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "sub") != NULL)
		{
			strcpy(functionalUnitTypes[3].unitType, "sub");
			functionalUnitTypes[3].numberOfUnits = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "mul") != NULL)
		{
			strcpy(functionalUnitTypes[4].unitType, "mul");
			functionalUnitTypes[4].numberOfUnits = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "div") != NULL) //div
		{
			strcpy(functionalUnitTypes[5].unitType, "div");
			functionalUnitTypes[5].numberOfUnits = getParameterFromString(lineInCfgFile);
		}
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

/************************************************************************************************
Description:
			This function updates the delay of units according to their type in the
			functionalUnitTypes array.
************************************************************************************************/
int addDelayOfUnit(char *lineInCfgFile, functionalUnit functionalUnitTypes[])
{
	if (strstr(lineInCfgFile, "delay") != NULL)
	{
		if (strstr(lineInCfgFile, "ld") != NULL)
		{
			functionalUnitTypes[0].unitDelay = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "st") != NULL)
		{
			functionalUnitTypes[1].unitDelay = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "add") != NULL)
		{
			functionalUnitTypes[2].unitDelay = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "sub") != NULL)
		{
			functionalUnitTypes[3].unitDelay = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "mul") != NULL)
		{
			functionalUnitTypes[4].unitDelay = getParameterFromString(lineInCfgFile);
		}
		else if (strstr(lineInCfgFile, "div") != NULL)//div
		{
			functionalUnitTypes[5].unitDelay = getParameterFromString(lineInCfgFile);
		}
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

/************************************************************************************************
Description:
			This function updates one line of configurations according to a line of cfg.txt.
************************************************************************************************/
void updateConfigurations(char *lineInCfgFile, functionalUnit functionalUnitTypes[], char traceUnit[])
{
	if (getTraceUnit(lineInCfgFile, traceUnit))
		return;
	else if (addNumOfUnits(lineInCfgFile, functionalUnitTypes))
		return;
	addDelayOfUnit(lineInCfgFile, functionalUnitTypes);
}

/************************************************************************************************
Description:
			This function updates all the configurations according to cfg.txt.
************************************************************************************************/
void getProgramCfgs(char *pathOfCfgFile, functionalUnit functionalUnitTypes[], char traceUnit[])
{
	FILE *CfgfilePtr = NULL;
	char lineInCfgFile[20] = "";
	CfgfilePtr = openFile(pathOfCfgFile, CfgfilePtr, READ_SELECT);
	while (feof(CfgfilePtr) == 0) //loop until the end of the file
	{
		fgets(lineInCfgFile, 20, CfgfilePtr); //get a line of the configutation file
		updateConfigurations(lineInCfgFile, functionalUnitTypes, traceUnit);
		strcpy(lineInCfgFile, "");
	}
	fclose(CfgfilePtr);//close Cfg file
}

/************************************************************************************************
Description:
			This function updates all the names of one unit type.
************************************************************************************************/
void updateUnitNames(int functionalUnitNum, char* unitType, unitStatus *unitStatusArray, int indOfUnitType, int opcode)
{
	int numOfUnitsInt = 0;
	char numOfUnitsChar = ' ', currUnitName[MAX_UNIT_NAME_LENGTH] = " ";
	for (numOfUnitsInt = 0; numOfUnitsInt < functionalUnitNum; numOfUnitsInt++) {
		numOfUnitsChar = numOfUnitsInt + '0';
		strcpy(currUnitName, unitType);
		strncat(currUnitName, &numOfUnitsChar, 1);
		strcpy(unitStatusArray[indOfUnitType + numOfUnitsInt].unitName, currUnitName);
		unitStatusArray[indOfUnitType + numOfUnitsInt].opcode = opcode;
		unitStatusArray[indOfUnitType + numOfUnitsInt].instInd = -1; //initialization
	}
}

/************************************************************************************************
Description:
			This function builds the units' status array.
************************************************************************************************/
unitStatus* buildUnitsStatusArray(char *pathOfCfgFile, functionalUnit functionalUnitTypes[], int *totalNumOfUnits,
	char traceUnit[])
{
	int i = 0, indOfUnitType = 0;
	*totalNumOfUnits = 0;
	unitStatus *unitStatusArray = NULL;
	char unitTypeNames[NUM_OF_UNIT_TYPES][MAX_UNIT_NAME_LENGTH] = { "LD", "ST", "ADD", "SUB", "MUL", "DIV" };
	getProgramCfgs(pathOfCfgFile, functionalUnitTypes, traceUnit);
	for (i = 0; i < NUM_OF_UNIT_TYPES; i++)
		*totalNumOfUnits = *totalNumOfUnits + functionalUnitTypes[i].numberOfUnits;
	unitStatusArray = (unitStatus*)malloc((*totalNumOfUnits) * sizeof(unitStatus));
	for (i = 0; i < NUM_OF_UNIT_TYPES; i++) { //loop for updating all units' names.
		updateUnitNames(functionalUnitTypes[i].numberOfUnits, unitTypeNames[i], unitStatusArray, indOfUnitType, i);
		indOfUnitType = indOfUnitType + functionalUnitTypes[i].numberOfUnits;
	}
	return unitStatusArray;
}


/************************************************************************************************
Description:
			This function deletes an instruction from the Units' status array.
************************************************************************************************/
void deleteInstructionFromUnitStatus(unitStatus* unitStatusArray, int indOfUnit)
{
	unitStatusArray[indOfUnit].Fi = -1; //update destination register
	unitStatusArray[indOfUnit].Fj = -1; //update src0 register
	unitStatusArray[indOfUnit].FjVal = -1; //update src0 register
	unitStatusArray[indOfUnit].Fk = -1; //update src1 register
	unitStatusArray[indOfUnit].FkVal = -1; //update src1 register
	unitStatusArray[indOfUnit].instInd = -1;//update instruction's index 
	strcpy(unitStatusArray[indOfUnit].Qj, "-");
	strcpy(unitStatusArray[indOfUnit].Qk, "-");
}


/************************************************************************************************
Description:
			This function finds an instruction in the Units' status array.
************************************************************************************************/
int findInsInUnitStatusArray(unitStatus* unitStatusArray, int totalNumOfUnits, int instructionInd)
{
	int indOfUnit = 0;
	for (indOfUnit = 0; indOfUnit < totalNumOfUnits; indOfUnit++)
	{
		if (unitStatusArray[indOfUnit].instInd == instructionInd)
			return indOfUnit;
	}
	return NO_INDEX_FOUND;
}

/************************************************************************************************
Description:
			This function inserts an instruction into the units' status array.
************************************************************************************************/
int insertInstructionIntoUnitStatus(unitStatus* unitStatusArray, int memCurrIns, int insPc, int totalNumOfUnits, registerStatus *registersArray, char unitName[])
{
	int Opcode = 0, Dst = 0, Src0 = 0, Src1 = 0, Imm = 0, ind = 0, busyDst = 0;
	int *pOpcode = &Opcode, *pDst = &Dst, *pSrc0 = &Src0, *pSrc1 = &Src1, *pImm = &Imm;
	defineRegisters(memCurrIns, pOpcode, pDst, pSrc0, pSrc1, pImm);
	for (ind = 0; ind < totalNumOfUnits; ind++)
	{
		if (*pOpcode != 1) //bustDst irrelevant if it is a ST instruction
			busyDst = strcmp(registersArray[*pDst].Qi, "none");
		if ((unitStatusArray[ind].instInd == -1) && (unitStatusArray[ind].opcode == *pOpcode) && !busyDst) //there is a free unit
		{
			strcpy(unitName, unitStatusArray[ind].unitName);
			unitStatusArray[ind].Fi = *pDst; //update destination register
			if (*pOpcode != 1)//not a ST instruction
				strcpy(registersArray[*pDst].Qi, unitStatusArray[ind].unitName);
			unitStatusArray[ind].Fj = *pSrc0; //update src0 register
			unitStatusArray[ind].FjVal = -1; //update src0 register value
			unitStatusArray[ind].Fk = *pSrc1;  //update src1 register
			unitStatusArray[ind].FkVal = -1;  //update src1 register value
			if (strcmp(registersArray[*pSrc0].Qi, "none") != 0) // we can't use Fj val- another ins need to write it before  
			{
				strcpy(unitStatusArray[ind].Qj, registersArray[*pSrc0].Qi);
				strcpy(unitStatusArray[ind].Rj, "No");
			}
			else
			{
				strcpy(unitStatusArray[ind].Rj, "Yes");
				strcpy(unitStatusArray[ind].Qj, "-");
			}

			if (strcmp(registersArray[*pSrc1].Qi, "none") != 0) // we can't use Fk val- another ins need to write it before  
			{
				strcpy(unitStatusArray[ind].Qk, registersArray[*pSrc1].Qi);
				strcpy(unitStatusArray[ind].Rk, "No");
			}
			else
			{
				strcpy(unitStatusArray[ind].Rk, "Yes");
				strcpy(unitStatusArray[ind].Qk, "-");
			}
			unitStatusArray[ind].instInd = insPc;
			return EXIT_SUCCESS; //we found a free unit
		}
	}
	return EXIT_ERROR; //we couldn't find a free unit or the Qi destination register is not empty 
}


/************************************************************************************************
Description:
			This function updates the values of Qj, Rj, Qk and Rk.
************************************************************************************************/
void updateQRjndQRk(unitStatus* unitStatusArray, int indOfUnit, registerStatus* registersArray)
{
	int src0Reg = 0, src1Reg = 0;
	if (strcmp(unitStatusArray[indOfUnit].Qj, "-") != 0)
	{
		src0Reg = unitStatusArray[indOfUnit].Fj;
		if (!strcmp(registersArray[src0Reg].Qi, "none") ||
			!strcmp(registersArray[src0Reg].Qi, unitStatusArray[indOfUnit].unitName)) //we can read src0 register
		{
			strcpy(unitStatusArray[indOfUnit].Qj, "-");
			strcpy(unitStatusArray[indOfUnit].Rj, "Yes");
		}
	}
	if (strcmp(unitStatusArray[indOfUnit].Qk, "-") != 0)
	{
		src1Reg = unitStatusArray[indOfUnit].Fk;
		if (!strcmp(registersArray[src1Reg].Qi, "none") ||
			!strcmp(registersArray[src1Reg].Qi, unitStatusArray[indOfUnit].unitName)) //we can read src1 register
		{
			strcpy(unitStatusArray[indOfUnit].Qk, "-");
			strcpy(unitStatusArray[indOfUnit].Rk, "Yes");
		}
	}
}

/************************************************************************************************
Description:
			This function finds the index of trace unit in the unit status array.
************************************************************************************************/
int findIndOfTraceUnit(unitStatus* unitStatusArray, int totalNumOfUnits, char traceUnit[])
{
	int ind = 0;
	for (ind = 0; ind < totalNumOfUnits; ind++)
	{
		if (!strcmp(unitStatusArray[ind].unitName, traceUnit)) //found the trace unit in unitStatusArray
			return ind;
	}
}
