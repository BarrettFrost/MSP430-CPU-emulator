
#include "instructions.h"

#define RIGHTSHIFTTWELVE 12
#define STATUSREGISTER 2
#define SIXTHBIT 5

/* iniatlise the registers by setting contents to zero*/
registers* registerInit(){
   int i;
   registers *r = (registers*)malloc(sizeof(registers));
   for(i = 0; i < REGISTERCOUNT; i++){
     r->registerArray[i] = 0x0000;
   }
   return r;
}

/*execute instruction from memory address in the program counter(r0) */
void instructionsExecute(uint16_t instruction, memory *ram, registers *r){
   uint16_t opcode = instruction >> RIGHTSHIFTTWELVE;
   if(opcode >= 4 && opcode <= 16){
    formatOneExecute(instruction, ram->memorySpace, r->registerArray, opcode);
   }
   else if(opcode  == 1){
     formatTwoExecute(instruction, ram->memorySpace, r->registerArray);
   }
   else if(opcode == 2 || opcode == 3){
     formatThreeExecute(instruction, r->registerArray);
   }
   else{
     printf("invalid instruction\n");
     r->registerArray[STATUSREGISTER] = r->registerArray[STATUSREGISTER] | (1 << SIXTHBIT);
     exit(1);
   }
}
