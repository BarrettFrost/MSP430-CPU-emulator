#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "memory.h"
#include "format1.h"
#include "format2.h"
#include "format3.h"
#define REGISTERCOUNT 16
struct registers{
    uint16_t registerArray[REGISTERCOUNT];
};

typedef struct registers registers;

registers* registerInit();

void instructionsExecute(uint16_t instruction, memory *ram, registers *r);
