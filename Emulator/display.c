#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "time.h"
#include "display.h"
#define STARTADDRESS 0x0500
#define DISPLAYCONTENTS 0x0400
#define PRINTREGISTERS 0x0700
#define ONESECOND 1000
#define PROGRAMCOUNTER 0
#define REGISTERCOUNT 16

/*prints a portion of memory to the console as an array*/
void displayArray(uint16_t *memorySpace, uint16_t *registerArray, int arraySize){
    int i, j;
    uint16_t address = STARTADDRESS, pc = registerArray[PROGRAMCOUNTER];
    if(pc == DISPLAYCONTENTS){
      for(i = 0; i < arraySize; i++){
        for(j = 0; j < arraySize; j++){
          if(memorySpace[address] != 0){
          printf("%u ", memorySpace[address]);
          }
          else{
            printf("  ");
          }
          address++;
        }
        printf("\n");
      }
      delay(ONESECOND);
      printf("\n");
    }
}

/*print registers from assembly instruction*/
void printRegistersInstruction(uint16_t *memorySpace , uint16_t *registerArray){
  int i;
  if(memorySpace[PRINTREGISTERS] == 1){
      for(i = 0; i < REGISTERCOUNT; i++){
        printf("r%d = %u\n",i, registerArray[i]);
      }
      printf("\n");
  }
}

/*create an time delay*/
void delay(int number_of_seconds)
{
   int milli_seconds = ONESECOND * number_of_seconds;

   clock_t start_time = clock();

   while (clock() < start_time + milli_seconds);
}
