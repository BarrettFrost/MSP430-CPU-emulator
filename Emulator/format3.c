
#include "format3.h"
#define STATUSREGISTER 2
#define PROGRAMCOUNTER 0
#define MEMORYSIZE 65536
#define RIGHTSHIFTTEN 10
#define RIGHTSHIFTONE 1
#define RIGHTSHIFTTWO 2
#define RIGHTSHIFTEIGHT 8
#define SIXTHBIT 5
#define TWOBITS_ONE 0x0003
#define THREEBITS_ONE 0x0007
#define NINEBITS_ONE 0x01FF
#define ONEBIT_ONE 0x0001

/*determines which format 3 instruction is executed*/
void formatThreeExecute(uint16_t instruction, uint16_t *registerArray){
   short offset = getOffset(instruction);
   uint16_t opcode = instruction >> RIGHTSHIFTTEN;
   uint16_t *r2 = &registerArray[STATUSREGISTER];
   uint16_t *r0 = &registerArray[PROGRAMCOUNTER];

   switch(opcode){
     case 8:
     executeJNE(offset, r2, r0);
     break;
     case 9:
     executeJEQ(offset, r2, r0);
     break;
     case 10:
     executeJNC(offset, r2, r0);
     break;
     case 11:
     executeJC(offset, r2, r0);
     break;
     case 12:
     executeJN(offset, r2, r0);
     break;
     case 13:
     executeJGE(offset, r2, r0);
     break;
     case 14:
     executeJL(offset, r2, r0);
     break;
     case 15:
     executeJMP(offset, r2, r0);
     break;
     default:
     printf("error format3 exit\n");
     *r2 = *r2 | (1 << SIXTHBIT);
     exit(EXIT_SUCCESS);
   }
   registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
}

/* change program counter based on offset if zerobit in status register is 0*/
void executeJNE(short offset, uint16_t *r2, uint16_t *r0){
   uint16_t zerobit = *r2 & TWOBITS_ONE;
   zerobit = zerobit >> RIGHTSHIFTONE;
   if(zerobit == 0){
      *r0 = *r0 + (2*offset);
   }
}

/* change program counter based on offset if zerobit in status register is 1*/
void executeJEQ(short offset, uint16_t *r2, uint16_t *r0){
   uint16_t zerobit = *r2 & TWOBITS_ONE;
   zerobit = zerobit >> RIGHTSHIFTONE;
   if(zerobit == 1){
      *r0 = *r0 + (2*offset);
    }
}

/* change program counter based on offset if carry bit in status register is 0*/
void executeJNC(short offset, uint16_t *r2, uint16_t *r0){
    uint16_t carryBit = *r2 & ONEBIT_ONE;
    if(carryBit == 0){
       *r0 = *r0 + (2*offset);
    }
}

/* change program counter based on offset if carry bit in status register is 1*/
void executeJC(short offset, uint16_t *r2, uint16_t *r0){
   uint16_t carryBit = *r2 & ONEBIT_ONE;
   if(carryBit == 1){
     *r0 = *r0 + (2*offset);
   }
}

/* change program counter based on offset if negative bit in status register is 1*/
void executeJN(short offset, uint16_t *r2, uint16_t *r0){
   uint16_t negativeBit = *r2 & THREEBITS_ONE;
   negativeBit = negativeBit >> RIGHTSHIFTTWO;
   if(negativeBit == 1){
     *r0 = *r0 + (2*offset);
   }
}

/* change program counter based on offset if negative bit and overflowBit in status register XOR is 0*/
/* indicating result was greater than or equal to source*/
void executeJGE(short offset, uint16_t *r2, uint16_t *r0){
   uint16_t negativeBit = *r2 & THREEBITS_ONE, overflowBit;
   negativeBit = negativeBit >> RIGHTSHIFTTWO;
   overflowBit = *r2 & NINEBITS_ONE;
   overflowBit = overflowBit >> RIGHTSHIFTEIGHT;
   if((negativeBit ^ overflowBit) == 0){
     *r0 = *r0 + (2*offset);
   }
}

/* change program counter based on offset if negative bit and overflowBit in status register XOR is 1*/
/* indicating result was less than source*/
void executeJL(short offset, uint16_t *r2, uint16_t *r0){
   uint16_t negativeBit = *r2 & THREEBITS_ONE, overflowBit;
   negativeBit = negativeBit >> RIGHTSHIFTTWO;
   overflowBit = *r2 & NINEBITS_ONE;
   overflowBit = overflowBit >> RIGHTSHIFTEIGHT;
   if((negativeBit ^ overflowBit) == 1){
     *r0 = *r0 + (2*offset);
   }
}

/* change program counter based on offset */
void executeJMP(short offset, uint16_t *r2, uint16_t *r0){
   *r0 = *r0 + (2*offset);
}
