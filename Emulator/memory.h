#include <stdio.h>
#include <stdint.h>
#include "stdlib.h"

#define MEMORYSIZE 65536

struct memory{
   uint16_t memorySpace[MEMORYSIZE];
};

typedef struct memory memory;

memory* memoryInit();

memory* memoryloadBinary(memory *ram, FILE *instructions);

/*long int findFileSize(FILE *instructions);*/
