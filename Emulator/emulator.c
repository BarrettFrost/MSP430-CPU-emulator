
#include "emulator.h"
#define FIVEBITS_ONE 0x001F
#define ARG_TYPE 2
#define NUMBER 3
#define PROGRAMCOUNTER 0
#define RIGHTSHIFTFIVE 5
#define STATUSREGISTER 2
#define RIGHTSHIFTFOUR 4
#define FOURBITS_ONE 0x000F

/*initialise the emulator by initialising the registers and memory and also loading the instructions into memory*/
emulation emulatorInit(FILE *instructions){
   emulation emulator;
   emulator.ram = memoryInit();
   emulator.ram = memoryloadBinary(emulator.ram, instructions);
   emulator.r = registerInit();
   return emulator;
}

/*prints the registers to the console*/
void printRegisters(registers *r){
   int i;
   printf("register\tunsigned\tsigned\tbyte signed\tbcd/hex\n");
   for(i = 0; i < REGISTERCOUNT; i++){
     printf("R%d\t\t%u\t\t%d\t\t%hi\t\t%u\n",i, r->registerArray[i],
     calc2sComplement(r->registerArray[i]), calcByte2sComplement(r->registerArray[i]),
     calcBCD(r->registerArray[i]));
   }
}

/*emulator gets the instruction from memory addressed in the program counter and executes*/
void emulatorExec(emulation emulator){
    uint16_t instruction, programCounter;
    programCounter = emulator.r->registerArray[PROGRAMCOUNTER];
    instruction = emulator.ram->memorySpace[programCounter];
    instructionsExecute(instruction, emulator.ram, emulator.r);
}

/*return true if CPUOFF bit in the status register is 1*/
bool isCPUOFF(emulation emulator){
   uint16_t r2 = emulator.r->registerArray[STATUSREGISTER];
   r2 = r2 & FIVEBITS_ONE ;
   r2 = r2 >> RIGHTSHIFTFOUR;
   if(r2 == 1){
     return true;
   }
   return false;
}

/*converts unsigned to 2s complement number*/
int calc2sComplement(uint16_t data){
     short complement = (short)data;
     return complement;
}

/*converts unsigned to byte 2s complement number*/
signed char calcByte2sComplement(uint16_t data){
     signed char complement = (signed char)data;
     return complement;
}

/*convert unsigned to BCD format*/
uint16_t calcBCD(uint16_t data){
     int factor = 1, i, digit;
     uint16_t bcd = 0;
     for(i = 0; i < 4; i++){
       if(i != 0){
          data = data >> RIGHTSHIFTFOUR;
       }
       digit = data & FOURBITS_ONE;
       bcd = bcd + (factor * digit);
       factor = factor * 10;
     }
     return bcd;
}


/*checks if the user has specified another arguement*/
bool isAdditionalArg(int argc, char const *argv[]){
  if(argv[ARG_TYPE] != NULL){
    if(argv[NUMBER] != NULL){
      return true;
    }
  }
  return false;
}
