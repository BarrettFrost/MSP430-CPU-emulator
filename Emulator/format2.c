
#include "format2.h"
#define MSBWORDPOSITION 15
#define MSBBYTEPOSITION 7
#define FORMAT2 2
#define STATUSREGISTER 2
#define STACKREGISTER 1
#define PROGRAMCOUNTER 0
#define RIGHTSHIFTSIX 6
#define EIGHTBITS_ONE 0x00FF
#define ONEBIT_ONE 0x0001
#define FIRSTBIT 0
#define RIGHTSHIFTFIFTHTEEN 15
#define SEVENBITS_ONE 0x007F
#define RIGHTSHIFTEIGHT 8
#define BYTE 8
#define RIGHTSHIFTSEVEN 7
#define RIGHTSHIFTONE 1
#define SIXTHBIT 5

/*determines which format 2 instruction is executed*/
void formatTwoExecute(uint16_t instruction, uint16_t *memorySpace, uint16_t *registerArray){
  uint16_t srcContent = 0, *dst = NULL;
  uint16_t byteWordBit = byteOrWord(instruction);
  uint16_t *r2 = &registerArray[STATUSREGISTER];
  uint16_t *r1 = &registerArray[STACKREGISTER];
  uint16_t *r0 = &registerArray[PROGRAMCOUNTER];
  uint16_t opcode = instruction & 0x0FFF;
  opcode = opcode >> RIGHTSHIFTSIX;
  if(opcode == 9 || opcode == 8){
    srcContent = getSrc(instruction, memorySpace, registerArray, FORMAT2);
  }
  else if(opcode != 12 && opcode != 10){
    dst = getDst(instruction, memorySpace, registerArray);
  }
  switch (opcode)
  {
    case 0:
    executeRRC(dst, byteWordBit, r2, MSBWORDPOSITION);
    break;
    case 1:
    executeRRC(dst, byteWordBit, r2, MSBBYTEPOSITION);
    break;
    case 4:
    executeRRA(dst, byteWordBit, r2, MSBWORDPOSITION);;
    break;
    case 5:
    executeRRA(dst, byteWordBit, r2, MSBBYTEPOSITION);
    break;
    case 2:
    executeSWPB(dst);
    break;
    case 6:
    executeSXT(dst, byteWordBit, r2);
    break;
    case 8:
    executePUSH(r1, srcContent, byteWordBit, memorySpace);
    break;
    case 9:
    executePUSH(r1, srcContent, byteWordBit, memorySpace);
    break;
    case 10:
    executeCALL(decodeCall(instruction, registerArray, memorySpace), r1, r0, memorySpace);
    break;
    case 12:
    executeRETI(r2, r1, r0, memorySpace);
    break;
    default:
    printf("error format2 exit\n");
    *r2 = *r2| (1 << SIXTHBIT);
    exit(EXIT_SUCCESS);
  }
  registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
}

/*Rotate right through carry*/
void executeRRC(uint16_t *dst, uint16_t byteWordBit, uint16_t *r2, int msbPostion){
   uint16_t carryBit = *r2 & ONEBIT_ONE;
   uint16_t lsb = *dst & ONEBIT_ONE;
   if(byteWordBit == 1){
     *dst = *dst & EIGHTBITS_ONE;
   }
   *dst = *dst >> RIGHTSHIFTONE;
   if(carryBit == 1){
     *dst = *dst | (1 << msbPostion);
   }
   else{
     *dst = *dst & ~(1 << msbPostion);
   }
   if(lsb == 1){
      *r2 = *r2 | (1 << FIRSTBIT);
   }
   else{
     *r2 = *r2 & ~(1 << FIRSTBIT);
   }
   resetOverflowBit(r2);
   changeNegativeBit(*dst, r2, byteWordBit);
   changeZeroBit(*dst, r2);
}

/*perform a arithmetically right shift*/
void executeRRA(uint16_t *dst, uint16_t byteWordBit, uint16_t *r2, int msbPostion){
  uint16_t lsb = *dst & ONEBIT_ONE, msb;
  if(byteWordBit == 0){
    msb = *dst >> RIGHTSHIFTFIFTHTEEN;
  }
  else{
    msb = *dst & EIGHTBITS_ONE;
    msb = msb >> RIGHTSHIFTSEVEN;
  }
  *dst = *dst >> 1;
  if(msb == 1){
    *dst = *dst | (1 << msbPostion);
  }
  else{
    *dst = *dst & ~(1 << msbPostion);
  }
  if(lsb == 1){
     *r2 = *r2 | (1 << FIRSTBIT);
  }
  else{
    *r2 = *r2 & ~(1 << FIRSTBIT);
  }
  resetOverflowBit(r2);
  changeNegativeBit(*dst, r2, byteWordBit);
  changeZeroBit(*dst, r2);
}

/*swap the contents of the low byte and high byte*/
void executeSWPB(uint16_t *dst){
   int i, j = 8;
   uint16_t lowByte = *dst & EIGHTBITS_ONE, bit;
   *dst = *dst >> RIGHTSHIFTEIGHT;
    for(i = 0; i < BYTE; i++){
        bit = (lowByte & ( 1 << i )) >> i;
       if(bit == 1){
         *dst = *dst | (1 << j);
       }
       else{
         *dst = *dst & ~(1 << j);
       }
       j++;
    }
}

/* extend the sign of low byte through the high byte*/
void executeSXT(uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
  int i;
  uint16_t msb = *dst & EIGHTBITS_ONE;
  msb = msb >> RIGHTSHIFTSEVEN;
  for(i = 8; i < 16; i++){
    if(msb == 1){
      *dst = *dst | (1 << i);
    }
    else{
      *dst = *dst & ~(1 << i);
    }
  }
  resetOverflowBit(r2);
  changeNegativeBit(*dst, r2, byteWordBit);
  changeZeroBit(*dst, r2);
}

/*push contents of on to stack in memory*/
void executePUSH(uint16_t *r1, uint16_t srcContent, uint16_t byteWordBit, uint16_t *memorySpace){
   uint16_t address;
   *r1 = *r1 - 2;
   address = *r1;
   if(byteWordBit == 0){
     memorySpace[address] = srcContent;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     memorySpace[address] = srcContent;
   }
}

/*call a function by placing PROGRAMCOUNTER on the the stack in memory and moving the destination contents in the program counter */
void executeCALL(uint16_t dst, uint16_t *r1, uint16_t *r0, uint16_t *memorySpace){
   *r1 = *r1 - 2;
   memorySpace[*r1] = *r0;
   *r0 = dst - 2;          /*pc incremented by 2 later*/
}

/*get the status register from the memory pointed to by the stack pointer then the same for the program counter*/
void executeRETI(uint16_t *r2, uint16_t *r1, uint16_t *r0, uint16_t *memorySpace){   /*CPU can't trigger softerware interrupts*/
   uint16_t tos = *r1;
   *r2 = memorySpace[tos];
   *r1 = *r1 + 2;
   tos = *r1;
   *r0 = memorySpace[tos];
   *r1 = *r1 + 2;
}
