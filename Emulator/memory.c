
#include "memory.h"
#define ONESIZE 1

/* all memory locations set to zero*/
memory* memoryInit(){
   int i;
   memory* ram = (memory*)malloc(sizeof(memory));
   for(i = 0; i < MEMORYSIZE; i++){
     ram->memorySpace[i] = 0x0000;
   }
   return ram;
}

/*read binary file and load into memory array*/
memory* memoryloadBinary(memory *ram, FILE *instructions){
   int i = 1;
   uint16_t instructionAddress = 0x0000;
   while(i == 1){
     i = fread(&ram->memorySpace[instructionAddress], sizeof(uint16_t), ONESIZE, instructions);
     instructionAddress = instructionAddress + 2;
   }
   return ram;
}
