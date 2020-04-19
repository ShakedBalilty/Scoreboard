#include "Instructions.h"

/************************************************************************************************
Description:
			This function copy opcode, dst, src0, src1 and imm from an instruction.
************************************************************************************************/
void defineRegisters(int memCurrline, int* pOpcode, int* pDst, int* pSrc0, int* pSrc1, int* pImm)
{
	int mask = 0x0f000000;
	*pOpcode = (int)(memCurrline & mask);
	*pOpcode >>= 24;
	mask = 0x00f00000;
	*pDst = (int)(memCurrline & mask);
	*pDst >>= 20;
	mask = 0x000f0000;
	*pSrc0 = (int)(memCurrline & mask);
	*pSrc0 >>= 16;
	mask = 0x0000f000;
	*pSrc1 = (int)(memCurrline & mask);
	*pSrc1 >>= 12;
	mask = 0x00000fff;
	*pImm = (int)(memCurrline & mask);
}


/************************************************************************************************
Description:
			This function implements the LD instruction (opcode number 0).
************************************************************************************************/
void opLd(int instruction, int mainMemoryArray[], registerStatus *regStatusArray)
{
	int immediate, destination, opcode, src0, src1;
	defineRegisters(instruction, &opcode, &destination, &src0, &src1, &immediate);
	regStatusArray[destination].registerVal = *((float *)mainMemoryArray + immediate);
}


/************************************************************************************************
Description:
			This function implements the ST instruction (opcode number 1).
************************************************************************************************/

void opSt(int instruction, int mainMemoryArray[], registerStatus *regStatusArray)
{
	int immediate, destination, opcode, src0, src1;
	defineRegisters(instruction, &opcode, &destination, &src0, &src1, &immediate);
	unsigned int *p = (unsigned int *)&regStatusArray[src1].registerVal;
	unsigned int single = *p;
	mainMemoryArray[immediate] = single;
}

/************************************************************************************************
Description:
			This function is used for add instruction (opcode number 2).
************************************************************************************************/
void opAdd(int dst, int src0, int src1, registerStatus *regStatusArray)
{
	regStatusArray[dst].registerVal = regStatusArray[src0].registerVal + regStatusArray[src1].registerVal;
}

/************************************************************************************************
Description:
			This function is used for sub instruction (opcode number 3).
************************************************************************************************/
void opSub(int dst, int src0, int src1, registerStatus *regStatusArray)
{
	regStatusArray[dst].registerVal = regStatusArray[src0].registerVal - regStatusArray[src1].registerVal;
}

/************************************************************************************************
Description:
			This function is used for mult instruction (opcode number 4).
************************************************************************************************/
void opMul(int dst, int src0, int src1, registerStatus *regStatusArray)
{
	regStatusArray[dst].registerVal = regStatusArray[src0].registerVal * regStatusArray[src1].registerVal;
}

/************************************************************************************************
Description:
			This function is used for mult instruction (opcode number 5).
************************************************************************************************/
void opDiv(int dst, int src0, int src1, registerStatus *regStatusArray)
{
	regStatusArray[dst].registerVal = regStatusArray[src0].registerVal / regStatusArray[src1].registerVal;
}

/************************************************************************************************
Description:
			This function is used for halt instruction (opcode number 6).
************************************************************************************************/
void opHalt()
{
}

/************************************************************************************************
Description:
			This function is used to execute the insruction.
************************************************************************************************/
void execInstruction(int instruction, int opcode, int dst, int src0, int src1, int mainMemoryArray[],
	registerStatus *regStatusArray)
{
	switch (opcode) {
	case OPCODE_LD:
		opLd(instruction, mainMemoryArray, regStatusArray);
		break;
	case OPCODE_ST:
		opSt(instruction, mainMemoryArray, regStatusArray);
		break;
	case OPCODE_ADD:
		opAdd(dst, src0, src1, regStatusArray);
		break;
	case OPCODE_SUB:
		opSub(dst, src0, src1, regStatusArray);
		break;
	case OPCODE_MULT:
		opMul(dst, src0, src1, regStatusArray);
		break;
	case OPCODE_DIV:
		opDiv(dst, src0, src1, regStatusArray);
		break;
	case OPCODE_HALT:
		opHalt();
		break;
	}
}
