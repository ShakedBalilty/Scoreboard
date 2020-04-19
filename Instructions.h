#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "memAndRegs.h"
#define OPCODE_LD 0
#define OPCODE_ST 1
#define OPCODE_ADD 2
#define OPCODE_SUB 3
#define OPCODE_MULT 4
#define OPCODE_DIV 5
#define OPCODE_HALT 6

void defineRegisters(int memCurrline, int* pOpcode, int* pDst, int* pSrc0, int* pSrc1, int* pImm);
void opLd(int instruction, int mainMemoryArray[], registerStatus *regStatusArray);
void opSt(int instruction, int mainMemoryArray[], registerStatus *regStatusArray);
void opAdd(int dst, int src0, int src1, registerStatus *regStatusArray);
void opSub(int dst, int src0, int src1, registerStatus *regStatusArray);
void opMul(int dst, int src0, int src1, registerStatus *regStatusArray);
void opDiv(int dst, int src0, int src1, registerStatus *regStatusArray);
void execInstruction(int instruction, int opcode, int dst, int src0, int src1, int mainMemoryArray[],
	registerStatus *regStatusArray);
#endif