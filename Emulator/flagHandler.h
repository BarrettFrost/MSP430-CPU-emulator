#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

void changeZeroBit(uint16_t result, uint16_t *r2);

void changeNegativeBit(uint16_t result, uint16_t *r2, uint16_t byteWordBit);

void changeCarryBit(uint16_t srcContent, uint16_t dstContent, uint16_t addition, uint16_t *r2, uint16_t byteWordBit, int wordCarry, int byteCarry);

void changeCarryForBitOp(uint16_t result, uint16_t *r2);

void changeCarryFromNegation(uint16_t srcContent, uint16_t addition, uint16_t *r2);

void changeCarryAddOne(uint16_t contents, uint16_t addition, uint16_t *r2, uint16_t byteOrWord, int wordCarry, int byteCarry);

void changeOverflowBit(uint16_t result, uint16_t *r2, uint16_t byteWordBit, uint16_t srcContent, uint16_t dstContent, bool isAdd);

bool isNegative(uint16_t result, uint16_t);

bool isCarry(uint16_t srcContent, uint16_t dstContent, uint16_t addition, uint16_t byteWordBit, int wordCarry, int byteCarry);

bool isOverflow(uint16_t result, uint16_t byteWordBit, uint16_t srcContent, uint16_t dstContent, bool isAdd);

void resetOverflowBit(uint16_t *r2);

uint16_t getMsb(uint16_t number, uint16_t byteWordBit);
