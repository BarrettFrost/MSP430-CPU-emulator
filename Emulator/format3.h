#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "decode.h"
#include "flagHandler.h"

void formatThreeExecute(uint16_t instruction, uint16_t *registerArray);

void executeJNE(short offset, uint16_t *r2, uint16_t *r0);

void executeJEQ(short offset, uint16_t *r2, uint16_t *r0);

void executeJNC(short offset, uint16_t *r2, uint16_t *r0);

void executeJC(short offset, uint16_t *r2, uint16_t *r0);

void executeJN(short offset, uint16_t *r2, uint16_t *r0);

void executeJGE(short offset, uint16_t *r2, uint16_t *r0);

void executeJL(short offset, uint16_t *r2, uint16_t *r0);

void executeJMP(short offset, uint16_t *r2, uint16_t *r0);
