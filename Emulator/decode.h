#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"


uint16_t getSrc(uint16_t instruction, uint16_t *memorySpace, uint16_t *registerArray, int format);

uint16_t* getDst(uint16_t instruction, uint16_t *memorySpace, uint16_t *registerArray);

uint16_t getSreg(uint16_t instruction, int format);

uint16_t getDreg(uint16_t instruction);

uint16_t byteOrWord(uint16_t instruction);

uint16_t getDstAddressMode(uint16_t instruction);

short getOffset(uint16_t instruction);

uint16_t decodeCall(uint16_t instruction, uint16_t *registerArray, uint16_t *memorySpace);
