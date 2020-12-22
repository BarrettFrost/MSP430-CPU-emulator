#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

void displayArray(uint16_t *memorySpace, uint16_t *registerArray, int arraySize);

void printRegistersInstruction(uint16_t *memorySpace , uint16_t *registerArray);

void delay(int number_of_seconds);
