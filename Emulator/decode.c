
#include "decode.h"
#define PROGRAMCOUNTER 0
#define FORMAT1 1
#define FORMAT2 2
#define SIXBITS_ONE 0x003F
#define RIGHTSHIFTFOUR 4
#define TWELVEBITS_ONE 0x0FFF
#define RIGHTSHIFTEIGHT 8
#define FOURBITS_ONE 0x000F
#define SEVENBITS_ONE 0X007F
#define RIGHTSHIFTSIX 6
#define EIGHTBITS_ONE 0x00FF
#define RIGHTSHIFTSEVEN 7
#define TENBITS_ONE 0x03FF
#define RIGHTSHIFTNINE 9
#define TENTHBIT 9
#define MSB_10BIT_2S -512
#define SIXTHBIT 5
#define STACKREGISTER 2

/*get the contents from the source specified from the instruction*/
uint16_t getSrc(uint16_t instruction, uint16_t *memorySpace, uint16_t *registerArray, int format){
   uint16_t address, src, bit, sreg, as;
   short index;
   as = instruction & SIXBITS_ONE;
   as = as >> RIGHTSHIFTFOUR;
   sreg = getSreg(instruction, format);
   if(as == 0){
      if(sreg == 3){
        return 0x0000;
      }
      return registerArray[sreg];
   }
   else if(as == 1 && sreg != 2 && sreg != 0){
      if(sreg == 3){
        return 0x0001;
      }
      registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
      address = registerArray[sreg];
      index = memorySpace[registerArray[PROGRAMCOUNTER]];
      src = address + index;
      return memorySpace[src];
   }
   else if(as == 1 && sreg == 0){
      registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
      address = registerArray[PROGRAMCOUNTER];
      index = memorySpace[registerArray[PROGRAMCOUNTER]];
      src = address + index;
      return memorySpace[src];
   }
   else if(as == 1 && sreg == 2){
      registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
      address = memorySpace[registerArray[PROGRAMCOUNTER]];
      return memorySpace[address];
   }
   else if(as == 2){
     if(sreg == 2){
       return 0x0004;
     }
     if(sreg == 3){
       return 0x0002;
     }
     address = registerArray[sreg];
     return memorySpace[address];
   }
   else if(as == 3 && sreg != 0){
     if(sreg == 2){
       return 0x0008;
     }
     if(sreg == 3){
       return 0xFFFF;
     }
     address = registerArray[sreg];
     bit = byteOrWord(instruction);
     if(bit == 0){
        registerArray[sreg] = registerArray[sreg] + 2;
        return memorySpace[address];
     }
     else{
        registerArray[sreg] = registerArray[sreg] + 1;
        return memorySpace[address];
     }
   }
   else if(as == 3 && sreg == 0){
      registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
      return memorySpace[registerArray[PROGRAMCOUNTER]];
   }
   else{
     printf("Error with src addressing mode\n");
     registerArray[STACKREGISTER] = registerArray[STACKREGISTER] | (1 << SIXTHBIT);
     exit(EXIT_SUCCESS);
   }
   return 0;
}

/*get the pointer address to the destination specified from the instruction*/
uint16_t* getDst(uint16_t instruction, uint16_t *memorySpace, uint16_t *registerArray){
   uint16_t mode = getDstAddressMode(instruction), dreg = getDreg(instruction), address, dst;
   short index;
   if(mode == 0){
      return &registerArray[getDreg(instruction)];
   }
   else if(mode == 1 && dreg != 2 && dreg != 0){
      registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
      address = registerArray[dreg];
      index = memorySpace[registerArray[PROGRAMCOUNTER]];
      dst = address + index;
      return &memorySpace[dst];
   }
   else if(mode == 1 && dreg == 0){
      registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
      address = registerArray[PROGRAMCOUNTER];
      index = memorySpace[registerArray[PROGRAMCOUNTER]];
      dst = address + index;
      return &memorySpace[dst];
   }
   else if(mode == 1 && dreg == 2){
      registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
      address = memorySpace[registerArray[PROGRAMCOUNTER]];
      return &memorySpace[address];
   }
   else{
     printf("Error with dst addressing mode\n");
     registerArray[STACKREGISTER] = registerArray[STACKREGISTER] | (1 << SIXTHBIT);
     exit(EXIT_SUCCESS);
   }
   return NULL;
}

/*get the pointer address to the destination specified from the call instruction*/
uint16_t decodeCall(uint16_t instruction, uint16_t *registerArray, uint16_t *memorySpace){
  uint16_t mode = getDstAddressMode(instruction), dreg = getDreg(instruction), dst, address;
  short index;
  if(mode == 1 && dreg != 2 && dreg != 0){
     registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
     address = registerArray[dreg];
     index = memorySpace[registerArray[PROGRAMCOUNTER]];
     dst = address + index;
     return dst;
  }
  else if(mode == 1 && dreg == 0){
     registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
     address = registerArray[PROGRAMCOUNTER];
     index = memorySpace[registerArray[PROGRAMCOUNTER]];
     dst = address + index;
     return dst;
  }
  else if(mode == 1 && dreg == 2){
     registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
     address = memorySpace[registerArray[PROGRAMCOUNTER]];
     return address;
  }
  else if(mode == 3 && dreg == 0){
     registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
     dst = memorySpace[registerArray[PROGRAMCOUNTER]];
     return dst;
  }
  else{
     dst = getSrc(instruction, memorySpace, registerArray, FORMAT2);
     return dst;
   }
}

/*get the source register specified in the instruction*/
uint16_t getSreg(uint16_t instruction , int format){
    uint16_t sreg;
    if(format == FORMAT1){
      sreg= instruction & TWELVEBITS_ONE;
      sreg = sreg >> RIGHTSHIFTEIGHT;
    }
    else{
      sreg = instruction & FOURBITS_ONE;
    }
    return sreg;
}

/*get the destination register specified in the instruction*/
uint16_t getDreg(uint16_t instruction){
    uint16_t dreg = instruction & FOURBITS_ONE;
    return dreg;
}

/*get the bit which specifies whether the instruction is word or byte operation*/
uint16_t byteOrWord(uint16_t instruction){
    uint16_t byteWordBit = instruction & SEVENBITS_ONE;
    byteWordBit = byteWordBit >> RIGHTSHIFTSIX;
    return byteWordBit;
}

/*get the addressing mode of the destination which is specified in the instruction*/
uint16_t getDstAddressMode(uint16_t instruction){
   uint16_t opcode = instruction >> 12;
   uint16_t mode;
   if(opcode != 1){
     mode = instruction & EIGHTBITS_ONE;
     mode = mode >> RIGHTSHIFTSEVEN;
     return mode;

   }
   else{
     mode = instruction & SIXBITS_ONE;
     mode = mode >> RIGHTSHIFTFOUR;
     return mode;
   }
}

/*get the offset from program counter specified in the instruction*/
short getOffset(uint16_t instruction){
   uint16_t offset = instruction & TENBITS_ONE;
   uint16_t tmp = offset >> RIGHTSHIFTNINE;
   short complement;
   offset = offset & ~(1 << TENTHBIT);
   if(tmp == 1){
      complement = MSB_10BIT_2S + offset;
      return complement;
   }
   return (short)offset;
}
