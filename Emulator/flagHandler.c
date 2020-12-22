#include "flagHandler.h"

#define SECONDBIT 1
#define THIRDBIT 2
#define FIRSTBIT 0
#define NINTHBIT 8
#define WORDMSB 15
#define BYTEMSB 7
#define BIT_OP_CARRY 0

/*change the zero bit in the status register*/
void changeZeroBit(uint16_t result, uint16_t *r2){
    if(result == 0){
      *r2 = *r2 | (1 << SECONDBIT);
    }
    else{
      *r2 = *r2 & ~(1 << SECONDBIT);
    }
}

/*change the negative bit in the status register*/
void changeNegativeBit(uint16_t result, uint16_t *r2, uint16_t byteWordBit){
   if(isNegative(result, byteWordBit)){
      *r2 = *r2 | (1 << THIRDBIT);
   }
   else{
     *r2 = *r2 & ~(1 << THIRDBIT);
   }
}

/*change the carry bit in the status register*/
void changeCarryBit(uint16_t srcContent, uint16_t dstContent, uint16_t addition, uint16_t *r2, uint16_t byteWordBit, int wordCarry, int byteCarry){
   if(isCarry(srcContent, dstContent, addition, byteWordBit, wordCarry, byteCarry)){
       *r2 = *r2 | (1 << FIRSTBIT);
   }
   else{
      *r2 = *r2 & ~(1 << FIRSTBIT);
   }
}

/*change the carry bit for bitwise operations*/
void changeCarryForBitOp(uint16_t result, uint16_t *r2){
  if(result != BIT_OP_CARRY){
      *r2 = *r2 | (1 << FIRSTBIT);
  }
  else{
     *r2 = *r2 & ~(1 << FIRSTBIT);
  }
}

/*change the overflow bit in the status register*/
void changeOverflowBit(uint16_t result, uint16_t *r2, uint16_t byteWordBit, uint16_t srcContent, uint16_t dstContent, bool isAdd){
  if(isOverflow(result, byteWordBit, srcContent, dstContent, isAdd)){
     *r2 = *r2 | (1 << NINTHBIT);
  }
  else{
    *r2 = *r2 & ~(1 << NINTHBIT);
  }
}

/*determine whether the result from operation is negative*/
bool isNegative(uint16_t result, uint16_t byteWordBit){
   uint16_t msb;
   if(byteWordBit == 0){
     msb = getMsb(result, byteWordBit);
     if(msb == 0){
       return false;
     }
     else{
       return true;
     }
   }
   else{
     msb = getMsb(result, byteWordBit);
     if(msb == 0){
       return false;
     }
     else{
       return true;
     }
   }
}

/*determine whether the result from operation has carry*/
bool isCarry(uint16_t srcContent, uint16_t dstContent, uint16_t addition, uint16_t byteWordBit, int wordCarry, int byteCarry){
   uint32_t result;
   result = srcContent + dstContent + addition;
   if(byteWordBit == 0){
     if((result >> 16) != 0 || wordCarry - srcContent < dstContent){
       return true;
     }
     else{
       return false;
     }
   }
   else{
     if((result >> 8) != 0 || byteCarry - srcContent < dstContent){
       return true;
     }
     else{
       return false;
     }
   }
}

/*determine whether the result from operation has an overflow*/
bool isOverflow(uint16_t result, uint16_t byteWordBit, uint16_t srcContent, uint16_t dstContent, bool isAdd){
   uint16_t msbSrc = getMsb(srcContent, byteWordBit);
   uint16_t msbDst = getMsb(dstContent, byteWordBit);
   uint16_t msbResult = getMsb(result, byteWordBit);
   if(isAdd){
      if(msbSrc == 0 && msbDst == 0 && msbResult == 1){
        return true;
      }
      else if(msbSrc == 1 && msbDst == 1 && msbResult == 0){
        return true;
      }
      else{
        return false;
      }
   }
   else{
     if(msbSrc == 0 && msbDst == 1 && msbResult == 0){
       return true;
     }
     else if(msbSrc == 1 && msbDst == 0 && msbResult == 1){
       return true;
     }
     else{
       return false;
     }
   }
}

/*set overflow bit to zero*/
void resetOverflowBit(uint16_t *r2){
   *r2 = *r2 & ~(1 << NINTHBIT);
}

/*get the most significant bit from uint16_t*/
uint16_t getMsb(uint16_t number, uint16_t byteWordBit){
    if(byteWordBit == 0){
      number = number >> WORDMSB;
      return number;
    }
    else{
      number = number >> BYTEMSB;
    }
    return number;
}
