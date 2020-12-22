#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "emulator.h"
#include "display.h"
#include "string.h"

#define BINFILE 1
#define PROGRAMCOUNTER 0
#define ARG_TYPE 2
#define NUMBER 3

int main(int argc, char const *argv[]) {
   FILE *instructions;
   emulation emulator;
   bool AdditionalArg;
   if(argc < 2){
     printf("no file\n");
     exit(EXIT_SUCCESS);
   }
   AdditionalArg = isAdditionalArg(argc, argv);
   instructions = fopen(argv[BINFILE],"rb");
   if(instructions == NULL){
      printf("FILE opening error");
      exit(EXIT_SUCCESS);
   }
   emulator = emulatorInit(instructions);
   while(!isCPUOFF(emulator)){
     emulatorExec(emulator);
     printRegistersInstruction(emulator.ram->memorySpace, emulator.r->registerArray);
     if(emulator.r->registerArray[PROGRAMCOUNTER] >= MEMORYSIZE){
       printf("Program counter out of memory\n");
       break;
     }
     if(AdditionalArg){
       if(strcmp("breakpoint", argv[ARG_TYPE]) == 0){
         if(emulator.r->registerArray[PROGRAMCOUNTER] >= atoi(argv[NUMBER])){
           break;
         }
       }
       if(strcmp("display", argv[ARG_TYPE]) == 0){
          displayArray(emulator.ram->memorySpace, emulator.r->registerArray, atoi(argv[NUMBER]));
       }
     }
   }
   printRegisters(emulator.r);
   fclose(instructions);
   return 0;
}
