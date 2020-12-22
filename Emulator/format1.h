#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "decode.h"
#include "stdbool.h"
#include "flagHandler.h"

void formatOneExecute(uint16_t instruction, uint16_t *memorySpace, uint16_t *registerArray, uint16_t opcode);

void executeMOV(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit);

void executeADD(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

void executeADDC(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

void executeSUB(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

void executeSUBC(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

void executeCMP(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

void executeDADD(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

uint8_t* getDigits(uint8_t *digits, uint16_t content, uint16_t byteWordBit);

uint16_t daddAddition(uint8_t *srcDigits, uint8_t *dstDigits, uint8_t carryBit, bool *isCarry, uint16_t byteWordBit);

uint16_t addFourBitsToResult(uint16_t result, uint8_t resultDigit, int bitPosition);

void executeBIT(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

void executeAND(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

void executeBIC(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit);

void executeBIS(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit);

void executeXOR(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);
