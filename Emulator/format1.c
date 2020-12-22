
#include "format1.h"
#define STATUSREGISTER 2
#define WORDCARRY 65535
#define BYTECARRY 255
#define DADDWORDCARRY 0x9999
#define DADDBYTECARRY 0x99
#define FORMAT1 1
#define PROGRAMCOUNTER 0
#define EIGHTBITS_ONE 0x00FF
#define ONEBIT_ONE 0x0001
#define SIXTHBIT 5
#define FIRSTBIT 0
#define FOURBITS_ONE 0x000F
#define RIGHTSHIFTFOUR 4

/*determines which format 1 instruction is executed*/
void formatOneExecute(uint16_t instruction, uint16_t *memorySpace, uint16_t *registerArray, uint16_t opcode){
   uint16_t srcContent = getSrc(instruction, memorySpace, registerArray, FORMAT1);
   uint16_t *dst = getDst(instruction, memorySpace, registerArray);
   uint16_t byteWordBit = byteOrWord(instruction);
   uint16_t *r2 = &registerArray[STATUSREGISTER];
   switch (opcode)
   {
     case 4:
     executeMOV(srcContent, dst, byteWordBit);
     break;
     case 5:
     executeADD(srcContent, dst, byteWordBit, r2);
     break;
     case 6:
     executeADDC(srcContent, dst, byteWordBit, r2);
     break;
     case 7:
     executeSUBC(srcContent, dst, byteWordBit, r2);
     break;
     case 8:
     executeSUB(srcContent, dst, byteWordBit, r2);
     break;
     case 9:
     executeCMP(srcContent, dst, byteWordBit, r2);
     break;
     case 0xA:
     executeDADD(srcContent, dst, byteWordBit, r2);
     break;
     case 0xB:
     executeBIT(srcContent, dst, byteWordBit, r2);
     break;
     case 0xC:
     executeBIC(srcContent, dst, byteWordBit);
     break;
     case 0xD:
     executeBIS(srcContent, dst, byteWordBit);
     break;
     case 0xE:
     executeXOR(srcContent, dst, byteWordBit, r2);
     break;
     case 0xF:
     executeAND(srcContent, dst, byteWordBit, r2);
     break;
     default:
     printf("error format1 exit\n");
     *r2 = *r2 | (1 << SIXTHBIT);
     exit(EXIT_SUCCESS);
   }
   registerArray[PROGRAMCOUNTER] = registerArray[PROGRAMCOUNTER] + 2;
}

/*moves the contents of source into the destination*/
void executeMOV(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit){
   if(byteWordBit == 0){
     *dst = srcContent;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     *dst = srcContent;
   }
}

/*adds the contents of source and the destination*/
void executeADD(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
   uint16_t dstContent = *dst;
   if(byteWordBit == 0){
     *dst = srcContent + dstContent;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     dstContent = dstContent & EIGHTBITS_ONE;
     *dst = srcContent + dstContent;
     *dst = *dst & EIGHTBITS_ONE;
   }
   changeOverflowBit(*dst,r2, byteWordBit, srcContent, dstContent, true);
   changeCarryBit(srcContent, dstContent, 0, r2, byteWordBit, WORDCARRY, BYTECARRY);
   changeZeroBit(*dst, r2);
   changeNegativeBit(*dst, r2, byteWordBit);
}

/*adds the contents of source, carryBit and the destination*/
void executeADDC(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
   uint16_t dstContent = *dst;
   uint16_t carryBit = *r2 & ONEBIT_ONE;
   if(byteWordBit == 0){
     *dst = srcContent + dstContent + carryBit;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     dstContent = dstContent & EIGHTBITS_ONE;
     *dst = srcContent + dstContent + carryBit;
     *dst = *dst & EIGHTBITS_ONE;
   }
   changeOverflowBit(*dst,r2, byteWordBit, srcContent, dstContent, true);
   changeCarryBit(srcContent, dstContent, carryBit, r2, byteWordBit, WORDCARRY, BYTECARRY);
   changeZeroBit(*dst, r2);
   changeNegativeBit(*dst, r2, byteWordBit);
}

/*subtracts the contents of source and the destination*/
void executeSUB(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
   uint16_t dstContent = *dst, result, complementSrc;
   uint8_t shortSrc;
   if(byteWordBit == 0){
     complementSrc = (~srcContent);
     *dst = dstContent + complementSrc + 1;
   }
   else{
     shortSrc = (uint8_t)srcContent;
     dstContent = (uint8_t)dstContent;
     shortSrc = ~shortSrc;
     complementSrc = shortSrc;
     result = dstContent + shortSrc +1;
     *dst = result;
     *dst = *dst & EIGHTBITS_ONE;
   }
   changeOverflowBit(*dst,r2, byteWordBit, srcContent, dstContent, false);
   changeCarryBit(complementSrc, dstContent, 1, r2, byteWordBit, WORDCARRY, BYTECARRY);
   changeZeroBit(*dst, r2);
   changeNegativeBit(*dst, r2, byteWordBit);
}

/*substracts the contents of source, carryBit and the destination*/
void executeSUBC(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
   uint16_t carryBit = *r2 & ONEBIT_ONE;
   uint16_t dstContent = *dst, result, complementSrc;
   uint8_t shortSrc;
   if(byteWordBit == 0){
     complementSrc = (~srcContent);
     *dst = dstContent + complementSrc + carryBit;;
   }
   else{
     shortSrc = (uint8_t)srcContent;
     dstContent = (uint8_t)dstContent;
     shortSrc = ~shortSrc;
     complementSrc = shortSrc;
     result = dstContent + shortSrc + carryBit;
     *dst = result;
     *dst = *dst & EIGHTBITS_ONE;
   }
   changeOverflowBit(*dst,r2, byteWordBit, srcContent, dstContent, false);
   changeCarryBit(complementSrc, dstContent, carryBit, r2, byteWordBit, WORDCARRY, BYTECARRY);
   changeZeroBit(*dst, r2);
   changeNegativeBit(*dst, r2, byteWordBit);
}

/*subtracts the contents of source and the destination doesn't save in destination*/
void executeCMP(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
  uint16_t dstContent = *dst, result, complementSrc;
  uint8_t shortSrc;
   if(byteWordBit == 0){
     complementSrc = (~srcContent);
     result = dstContent + complementSrc + 1;
   }
   else{
     shortSrc = (uint8_t)srcContent;
     dstContent = (uint8_t)dstContent;
     shortSrc = ~shortSrc;
     complementSrc = shortSrc;
     result = dstContent + shortSrc + 1;
     result = result & EIGHTBITS_ONE;
   }
   changeOverflowBit(result,r2, byteWordBit, srcContent, dstContent, false);
   changeCarryBit(complementSrc, dstContent, 1, r2, byteWordBit, WORDCARRY, BYTECARRY);
   changeZeroBit(result, r2);
   changeNegativeBit(result, r2, byteWordBit);
}

/*adds the contents of source, carryBit and the destination decimally*/
void executeDADD(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
   uint16_t dstContent = *dst;
   uint16_t carryBit = *r2 & ONEBIT_ONE;
   uint8_t *srcDigits = (uint8_t*)malloc(4*sizeof(uint8_t)), *dstDigits = (uint8_t*)malloc(4*sizeof(uint8_t));
   bool *isCarry = (bool*)malloc(sizeof(bool));
   if(byteWordBit == 0){
      srcDigits = getDigits(srcDigits ,srcContent, byteWordBit);
      dstDigits = getDigits(dstDigits, dstContent, byteWordBit);
      *dst = daddAddition(srcDigits, dstDigits, carryBit, isCarry, byteWordBit);
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     dstContent = dstContent & EIGHTBITS_ONE;
     srcDigits = getDigits(srcDigits, srcContent, byteWordBit);
     dstDigits = getDigits(dstDigits, dstContent, byteWordBit);
     *dst = daddAddition(srcDigits, dstDigits, carryBit, isCarry, byteWordBit);
     *dst = *dst & EIGHTBITS_ONE;
   }
   free(srcDigits);
   free(dstDigits);
   if(*isCarry){
     *r2 = *r2 | (1 << FIRSTBIT);
   }
   else{
     *r2 = *r2 & ~(1 << FIRSTBIT);
   }
   changeZeroBit(*dst, r2);
   changeNegativeBit(*dst, r2, byteWordBit);
   free(isCarry);
}

/*puts each 4 bits in a number in an array*/
uint8_t* getDigits(uint8_t *digits, uint16_t content, uint16_t byteWordBit){
   int i, max;
   uint8_t digit;
   if(byteWordBit == 0){
     max = 4;
   }
   else{
     max = 2;
   }
  for(i = 0; i < max; i++){
     if(i != 0){
        content = content >> RIGHTSHIFTFOUR;
     }
     digit = content & FOURBITS_ONE;
     digits[i] = digit;
   }
   return digits;
}

/*performs an addition decimally*/
uint16_t daddAddition(uint8_t *srcDigits, uint8_t *dstDigits, uint8_t carryBit, bool *isCarry, uint16_t byteWordBit){
   int i, max, bitPosition = 0;
   uint16_t result = 0;
   uint8_t resultDigit;
   if(byteWordBit == 0){
     max = 4;
   }
   else{
     max = 2;
   }
   for(i = 0; i < max; i++){
     resultDigit = srcDigits[i] + dstDigits[i] + carryBit;
     if(resultDigit > 9){
       *isCarry = true;
       carryBit = 1;
       resultDigit = resultDigit + 6;
     }
     else{
       *isCarry = false;
       carryBit = 0;
     }
     result = addFourBitsToResult(result, resultDigit, bitPosition);
     bitPosition = bitPosition + 4;
   }
   return result;
}

/*add 4 bits the 16 bit variable at specified position*/
uint16_t addFourBitsToResult(uint16_t result, uint8_t resultDigit, int bitPosition){
   int i, j = 0, bit, max = bitPosition + 4;
   for(i = bitPosition; i < max; i++){
     bit = (resultDigit >> j) & 1;
     if(bit == 1){
       result = result | (1 << i);
     }
     else{
       result = result & ~(1 << i);
     }
     j++;
   }
   return result;
}

/*perform an AND operation on the contents of source and the destination*/
void executeBIT(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
   uint16_t dstContent = *dst, result;
   if(byteWordBit == 0){
     result = srcContent & dstContent;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     dstContent = dstContent & EIGHTBITS_ONE;
     result = srcContent & dstContent;
   }
   resetOverflowBit(r2);
   changeCarryForBitOp(result, r2);
   changeZeroBit(result, r2);
   changeNegativeBit(result, r2, byteWordBit);
}

/*perform an AND operation on the contents of source and the destination keeping the results in the destination*/
void executeAND(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
   uint16_t dstContent = *dst;
   if(byteWordBit == 0){
     *dst = srcContent & dstContent;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     dstContent = dstContent & EIGHTBITS_ONE;
     *dst = srcContent & dstContent;
     *dst = *dst & EIGHTBITS_ONE;
   }
   resetOverflowBit(r2);
   changeCarryForBitOp(*dst, r2);
   changeZeroBit(*dst, r2);
   changeNegativeBit(*dst, r2, byteWordBit);
}

/*perform an AND operation on the NOT contents of source and the destination keeping the results in the destination and not affecting the status register*/
void executeBIC(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit){
   uint16_t dstContent = *dst;
   if(byteWordBit == 0){
     *dst = (~srcContent) & dstContent;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     dstContent = dstContent & EIGHTBITS_ONE;
     *dst = (~srcContent) & dstContent;
     *dst = *dst & EIGHTBITS_ONE;
   }
}

/*perform an OR operation on the contents of source and the destination keeping the results in the destination and not affecting the status register*/
void executeBIS(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit){
   uint16_t dstContent = *dst;
   if(byteWordBit == 0){
     *dst = srcContent | dstContent;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     dstContent = dstContent & EIGHTBITS_ONE;
     *dst = srcContent | dstContent;
     *dst = *dst & EIGHTBITS_ONE;
   }
}

/*perform an XOR operation on the contents of source and the destination keeping the results in the destination */
void executeXOR(uint16_t srcContent, uint16_t *dst, uint16_t byteWordBit, uint16_t *r2){
   uint16_t dstContent = *dst;
   if(byteWordBit == 0){
     *dst = srcContent ^ dstContent;
   }
   else{
     srcContent = srcContent & EIGHTBITS_ONE;
     dstContent = dstContent & EIGHTBITS_ONE;
     *dst = srcContent ^ dstContent;
     *dst = *dst & EIGHTBITS_ONE;
   }
   resetOverflowBit(r2);
   changeCarryForBitOp(*dst, r2);
   changeZeroBit(*dst, r2);
   changeNegativeBit(*dst, r2, byteWordBit);
}
