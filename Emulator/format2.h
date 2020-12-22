#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "decode.h"
#include "stdbool.h"
#include "flagHandler.h"

void formatTwoExecute(uint16_t instruction, uint16_t *memorySpace, uint16_t *registerArray);

void executeRRC(uint16_t *dst, uint16_t byteWordBit, uint16_t *r2, int msbPostion);

void executeRRA(uint16_t *dst, uint16_t byteWordBit, uint16_t *r2, int msbPostion);

void executeSWPB(uint16_t *dst);

void executeSXT(uint16_t *dst, uint16_t byteWordBit, uint16_t *r2);

void executePUSH(uint16_t *r1, uint16_t srcContent, uint16_t byteWordBit, uint16_t *memorySpace);

void executeCALL(uint16_t dst, uint16_t *r1, uint16_t *r0, uint16_t *memorySpace);

void executeRETI(uint16_t *r2, uint16_t *r1, uint16_t *r0, uint16_t *memorySpace);
