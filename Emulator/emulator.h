
#include <stdio.h>
#include <stdint.h>
#include "stdlib.h"
#include <stdbool.h>
#include "instructions.h"

struct emulation{
   memory *ram;
   registers *r;
};

typedef struct emulation emulation;

emulation emulatorInit(FILE *instructions);

void emulatorExec(emulation emulator);

void printRegisters(registers *r);

bool isCPUOFF(emulation emulator);

int calc2sComplement(uint16_t data);

signed char calcByte2sComplement(uint16_t data);

uint16_t calcBCD(uint16_t data);

bool isAdditionalArg(int argc, char const *argv[]);
