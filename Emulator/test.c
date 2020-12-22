#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "emulator.h"
#include "assert.h"
#define STATUSREGISTER 2
#define PROGRAMCOUNTER 0
#define WORDCARRY 65536
#define BYTECARRY 256

void testFormat3();
void testFormat2();
void testFormat1();
void testflagHandler();
void testdecode();
void testemulator();
void testinstructions();
void testEmulatedInstructions();
void testMemory();
void testDADDFunctions();
int main() {
  testFormat3();
  testFormat2();
  testFormat1();
  testflagHandler();
  testdecode();
  testemulator();
  testinstructions();
  testEmulatedInstructions();
  testDADDFunctions();
  printf("tests were sucessful\n");
  return 0;
}

void testFormat3(){
   registers *r;
   uint16_t *r2, *r0;
   r = registerInit();
   r2 = &r->registerArray[STATUSREGISTER];
   r0 = &r->registerArray[PROGRAMCOUNTER];

   *r0 = 0;
   *r2 = 0;
   executeJNE(0x0004, r2, r0);
   assert(*r0 == 0x0008);
   executeJEQ(0x0004, r2, r0);
   assert(*r0 == 0x0008);
   *r2 = *r2 | (1 << 1);         /*zero bit set*/
   executeJNE(0x0004, r2, r0);
   assert(*r0 == 0x0008);
   executeJEQ(0x0004, r2, r0);
   assert(*r0 == 0x0010);

   executeJNC(0x0004, r2, r0);
   assert(*r0 == 0x0018);
   executeJC(0x0004, r2, r0);
   assert(*r0 == 0x0018);
   *r2 = *r2 | (1 << 0);
   executeJNC(0x0004, r2, r0);
   assert(*r0 == 0x0018);
   executeJC(0x0004, r2, r0);
   assert(*r0 == 0x0020);

   executeJN(0x0004, r2, r0);
   assert(*r0 == 0x0020);
   *r2 = *r2 | (1 << 2);
   executeJN(0x0004, r2, r0);
   assert(*r0 == 0x0028);

   *r2 = *r2 | (1 << 8);
   *r2 = *r2 | (1 << 2);
   executeJGE(0x0004, r2, r0);
   assert(*r0 == 0x0030);
   *r2 = *r2 & ~(1 << 8);
   *r2 = *r2 | (1 << 2);
   executeJGE(0x0004, r2, r0);
   assert(*r0 == 0x0030);
   *r2 = *r2 | (1 << 8);
   *r2 = *r2 & ~(1 << 2);
   executeJGE(0x0004, r2, r0);
   assert(*r0 == 0x0030);
   *r2 = *r2 & ~(1 << 8);
   *r2 = *r2 & ~(1 << 2);
   executeJGE(0x0004, r2, r0);
   assert(*r0 == 0x0038);

   *r2 = *r2 | (1 << 8);
   *r2 = *r2 | (1 << 2);
   executeJL(0x0004, r2, r0);
   assert(*r0 == 0x0038);
   *r2 = *r2 & ~(1 << 8);
   *r2 = *r2 | (1 << 2);
   executeJL(0x0004, r2, r0);
   assert(*r0 == 0x0040);
   *r2 = *r2 | (1 << 8);
   *r2 = *r2 & ~(1 << 2);
   executeJL(0x0004, r2, r0);
   assert(*r0 == 0x0048);
   *r2 = *r2 & ~(1 << 8);
   *r2 = *r2 & ~(1 << 2);
   executeJL(0x0004, r2, r0);
   assert(*r0 == 0x0048);

   executeJMP(0x0004,r2, r0);
   assert(*r0 == 0x0050);

   *r2 = *r2 & ~(1 << 1);
   formatThreeExecute(0x2004, r->registerArray);
   assert(*r0 == 0x005A);
   *r2 = *r2 | (1 << 1);
   formatThreeExecute(0x2404, r->registerArray);
   assert(*r0 == 0x0064);
   *r2 = *r2 & ~(1 << 0);
   formatThreeExecute(0x2804, r->registerArray);
   assert(*r0 == 0x006E);
   *r2 = *r2 | (1 << 0);
   formatThreeExecute(0x2C04, r->registerArray);
   assert(*r0 == 0x0078);
   *r2 = *r2 | (1 << 2);
   formatThreeExecute(0x3004, r->registerArray);
   assert(*r0 == 0x0082);
   *r2 = *r2 | (1 << 2);
   formatThreeExecute(0x3004, r->registerArray);
   assert(*r0 == 0x008C);
   *r2 = *r2 | (1 << 8);;
   formatThreeExecute(0x3404, r->registerArray);
   assert(*r0 == 0x0096);
   *r2 = *r2 & ~(1 << 8);
   formatThreeExecute(0x3804, r->registerArray);
   assert(*r0 == 0x00A0);
   formatThreeExecute(0x3C04, r->registerArray);
   assert(*r0 == 0x00AA);

   *r0 = 0x4444;
   *r2 = *r2 & ~(1 << 1);
   formatThreeExecute(0x23FF, r->registerArray); /*JNE*/
   assert(*r0 == 0x4444);
   *r2 = *r2 | (1 << 1);
   formatThreeExecute(0x27FF, r->registerArray);
   assert(*r0 == 0x4444);
   *r2 = *r2 & ~(1 << 0);
   formatThreeExecute(0x2BFF, r->registerArray);
   assert(*r0 == 0x4444);
   *r2 = *r2 | (1 << 0);
   formatThreeExecute(0x2FFF, r->registerArray);
   assert(*r0 == 0x4444);
   *r2 = *r2 | (1 << 2);
   formatThreeExecute(0x33FF, r->registerArray);
   assert(*r0 == 0x4444);
   *r2 = *r2 | (1 << 2);
   formatThreeExecute(0x33FF, r->registerArray);
   assert(*r0 == 0x4444);
   *r2 = *r2 | (1 << 8);;
   formatThreeExecute(0x37FF, r->registerArray);
   assert(*r0 == 0x4444);
   *r2 = *r2 & ~(1 << 8);
   formatThreeExecute(0x3BFF, r->registerArray);
   assert(*r0 == 0x4444);
   formatThreeExecute(0x3FFF, r->registerArray);
   assert(*r0 == 0x4444);

   *r0 = 0x0008;
   *r2 = 0;
   executeJNE(-4, r2, r0);
   assert(*r0 == 0x0000);

   *r0 = 0x0008;
   *r2 = 0;
   executeJNE(-4, r2, r0);
   assert(*r0 == 0x0000);

   *r0 = 0x0008;
   *r2 = *r2 | (1 << 1);
   executeJEQ(-4, r2, r0);
   assert(*r0 == 0x0000);

   *r0 = 0x0008;
   *r2 = 0;
   executeJNC(-4, r2, r0);
   assert(*r0 == 0x0000);

   *r0 = 0x0008;
   *r2 = *r2 | (1 << 0);
   executeJC(-4, r2, r0);
   assert(*r0 == 0x0000);

   *r0 = 0x0008;
   *r2 = *r2 | (1 << 2);
   executeJN(-4, r2, r0);
   assert(*r0 == 0x0000);

   *r0 = 0x0008;
   *r2 = *r2 | (1 << 8);
   *r2 = *r2 | (1 << 2);
   executeJGE(-4, r2, r0);
   assert(*r0 == 0x0000);

   *r0 = 0x0008;
   *r2 = *r2 & ~(1 << 8);
   *r2 = *r2 | (1 << 2);
   executeJL(-4, r2, r0);
   assert(*r0 == 0x0000);

   *r0 = 0x0008;
   executeJMP(-4, r2, r0);
   assert(*r0 == 0x0000);
}

void testFormat2(){
   memory *ram;
   registers *r;
   uint16_t *r2, *r0, *r4, *r1;
   ram = memoryInit();
   r = registerInit();
   r4 = &r->registerArray[4];
   r2 = &r->registerArray[2];
   r1 = &r->registerArray[1];
   r0 = &r->registerArray[0];

   *r4 = 0x00FF;
   executeSWPB(r4);
   assert(*r4 == 0xFF00);
   *r4 = 0xFF00;
   executeSWPB(r4);
   assert(*r4 == 0x00FF);
   *r4 = 0x8844;
   executeSWPB(r4);
   assert(*r4 == 0x4488);
   *r4 = 0x0000;
   executeSWPB(r4);
   assert(*r4 == 0x0000);

   *r4 = 0x0F0F;
   *r2 = 256;
   executeRRC(r4, 0, r2, 15);   /*reset overflow*/
   assert(*r4 == 0x0787);
   assert(*r2 == 0x0001);
   *r4 = 0x0001;
   *r2 = 0;
   executeRRC(r4, 0, r2, 15);
   assert(*r4 == 0x0000);          /*zero and carry*/
   assert(*r2 == 0x0003);
   *r4 = 0x0FF0;
   executeRRC(r4, 0, r2, 15);
   assert(*r4 == 0x87F8);
   assert(*r2 == 0x0004);          /* negative no carry*/
   *r4 = 0xF0F;
   *r2 = 256;
   executeRRC(r4, 1, r2, 7);
   assert(*r4 == 0x07);
   assert(*r2 == 0x0001);
   *r4 = 0x01;
   *r2 = 0;
   executeRRC(r4, 1, r2, 15);
   assert(*r4 == 0x00);          /*zero and carry*/
   assert(*r2 == 0x0003);
   *r4 = 0x70;
   executeRRC(r4, 1, r2, 7);   /* negative no carry*/
   assert(*r4 == 0xB8);
   assert(*r2 == 0x0004);

   *r4 = 0x0F0F;
   *r2 = 256;
   executeRRA(r4, 0, r2, 15);
   assert(*r4 == 0x0787);
   assert(*r2 == 0x0001);
   *r4 = 0x0FF0;
   executeRRA(r4, 0, r2, 15);
   assert(*r4 == 0x07F8);
   assert(*r2 == 0x0000);
   *r4 = 0x0001;
   executeRRA(r4, 0, r2, 15);
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);     /*zero and carry*/
   *r4 = 0x8FF0;
   executeRRA(r4, 0, r2, 15);
   assert(*r4 == 0xC7F8);
   assert(*r2 == 0x0004);     /*negative*/
   *r4 = 0x0F;
   *r2 = 256;
   executeRRA(r4, 1, r2, 7);
   assert(*r4 == 0x07);
   assert(*r2 == 0x0001);
   *r4 = 0x70;
   executeRRA(r4, 1, r2, 7);
   assert(*r4 == 0x38);
   assert(*r2 == 0x0000);
   *r4 = 0x01;
   executeRRA(r4, 1, r2, 7);
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);     /*zero and carry*/
   *r4 = 0x88;
   executeRRA(r4, 1, r2, 7);
   assert(*r4 == 0xC4);
   assert(*r2 == 0x0004);     /*negative*/

   *r4 = 0x007F;
   *r2 = 256;
   executeSXT(r4, 0, r2);
   assert(*r4 == 0x007F);   /*reset overflow*/
   assert(*r2 == 0x0000);
   *r4 = 0x00FF;
   executeSXT(r4, 0, r2);
   assert(*r4 == 0xFFFF);   /*negative*/
   assert(*r2 == 0x0004);
   *r4 = 0xFF00;
   executeSXT(r4, 0, r2);
   assert(*r4 == 0x0000);   /*zero*/
   assert(*r2 == 0x0002);

   *r1 = 0x0008;
   executePUSH(r1, 0x0001 ,0, ram->memorySpace);
   assert(*r1 == 0x0006);
   assert(ram->memorySpace[0x0006] == 0x0001);

   *r1 = 0x0000;
   executePUSH(r1, 0x0002 ,0, ram->memorySpace);
   assert(*r1 == 0xFFFE);
   assert(ram->memorySpace[0xFFFE] == 0x0002);

   *r1 = 0x0006;
   executePUSH(r1, 0x1001 ,1, ram->memorySpace);
   assert(*r1 == 0x0004);
   assert(ram->memorySpace[0x0004] == 0x0001);

   *r1 = 0x0000;
   executePUSH(r1, 0x1001 ,1, ram->memorySpace);
   assert(*r1 == 0xFFFE);
   assert(ram->memorySpace[0xFFFE] == 0x0001);

   *r0 = 0x00FF;
   *r1 = 0x0004;
   executeCALL(0x0000, r1, r0, ram->memorySpace);
   assert(*r1 == 0x0002);
   assert(ram->memorySpace[0x0002] == 0x00FF);
   assert(*r0 == 0xFFFE);

   *r0 = 0x0010;
   *r1 = 0x0000;
   executeCALL(0x000A, r1, r0, ram->memorySpace);
   assert(*r1 == 0xFFFE);
   assert(ram->memorySpace[0xFFFE] == 0x0010);
   assert(*r0 == 0x0008);

   *r1 = 0x0008;
   ram->memorySpace[0x0008] = 0x00FF;
   ram->memorySpace[0x000A] = 0x00C4;
   executeRETI(r2, r1, r0, ram->memorySpace);
   assert(*r1 == 0x000C);
   assert(*r2 == 0x00FF);
   assert(*r0 == 0x00C4);

   *r1 = 0xFFFE;
   ram->memorySpace[0xFFFE] = 0x00FF;
   ram->memorySpace[0x0000] = 0x00C4;
   executeRETI(r2, r1, r0, ram->memorySpace);
   assert(*r1 == 0x0002);
   assert(*r2 == 0x00FF);
   assert(*r0 == 0x00C4);

   *r4 = 0x0F0F;
   *r2 = 0x0000;
   formatTwoExecute(0x1004, ram->memorySpace, r->registerArray);
   assert(*r4 == 0x0787);
   assert(*r2 == 0x0001);

   *r4 = 0x0F;
   *r2 = 0x0000;
   formatTwoExecute(0x1044, ram->memorySpace, r->registerArray);
   assert(*r4 == 0x07);
   assert(*r2 == 0x0001);

   *r4 = 0x00FF;
   formatTwoExecute(0x1084, ram->memorySpace, r->registerArray);
   assert(*r4 == 0xFF00);

   *r4 = 0x0FF0;
   formatTwoExecute(0x1104, ram->memorySpace, r->registerArray);
   assert(*r4 == 0x07F8);
   assert(*r2 == 0x0000);

   *r4 = 0x0F;
   formatTwoExecute(0x1144, ram->memorySpace, r->registerArray);
   assert(*r4 == 0x07);
   assert(*r2 == 0x0001);

   *r1 = 0x0008;
   *r4 = 0x0001;
   formatTwoExecute(0x1204, ram->memorySpace, r->registerArray);
   assert(*r1 == 0x0006);
   assert(ram->memorySpace[0x0006] == 0x0001);

   formatTwoExecute(0x1244, ram->memorySpace, r->registerArray);
   assert(*r1 == 0x0004);
   assert(ram->memorySpace[0x0004] == 0x0001);

   *r0 = 0x00FF;
   *r4 = 0x0000;
   formatTwoExecute(0x1284, ram->memorySpace, r->registerArray);
   assert(*r1 == 0x0002);
   assert(ram->memorySpace[0x0002] == 0x00FF);
   assert(*r0 == 0x0000);


   formatTwoExecute(0x1304, ram->memorySpace, r->registerArray);
   assert(*r1 == 0x0006);
   assert(*r2 == 0x00FF);
   assert(*r0 == 0x0003);  /* PROGRAMCOUNTER +2*/
}

void testFormat1(){
   memory *ram;
   registers *r;
   uint16_t *r2, *r0, *r4;
   ram = memoryInit();
   r = registerInit();
   r4 = &r->registerArray[4];
   r2 = &r->registerArray[2];
   r0 = &r->registerArray[0];

   executeMOV(0x0004, r4 ,0);
   assert(*r4 == 0x0004);
   executeMOV(0x0FFF, r4 ,1);
   assert(*r4 == 0x00FF);
   executeMOV(0x8000, r4 ,0);
   assert(*r4 == 0x8000);
   executeMOV(0x00, r4 ,1);
   assert(*r4 == 0x0000);

   *r4 = 0x0004;
   executeADD(0x0004, r4, 0, r2);
   assert(*r4 == 0x0008);
   assert(*r2 == 0x0000);
   *r4 = 0xFFFF;
   executeADD(0xFFFF, r4, 0, r2);   /*minus minus*/
   assert(*r4 == 0xFFFE);
   assert(*r2 == 0x0005);
   *r4 = 0x0002;
   executeADD(0xFFFF, r4, 0, r2);   /*positive minus*/
   assert(*r4 == 0x0001);
   assert(*r2 == 0x0001);
   *r4 = 0xFFFD;
   executeADD(0x0001, r4, 0, r2);   /* minus postive*/
   assert(*r4 == 0xFFFE);
   assert(*r2 == 0x0004);
   *r4 = 0xFFFE;
   executeADD(0x0002, r4, 0, r2);   /*zero status*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);          /*zero bit plus carry bit*/
   *r4 = 0xFFFE;
   executeADD(0x0001, r4, 0, r2);   /*negative status*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);
   *r4 = 0xFFFF;
   executeADD(0x0002, r4, 0, r2);   /*carry status*/
   assert(*r4 == 0x0001);
   assert(*r2 == 0x0001);
   *r4 = 0x7FFF;
   executeADD(0x1000, r4, 0, r2);   /*overflow status*/
   assert(*r4 == 0x8FFF);
   assert(*r2 == 0x0104);         /*overflow and negative*/
   *r4 = 0xFFFF;
   executeADD(0x8000, r4, 0, r2);   /*overflow status*/
   assert(*r4 == 0x7FFF);
   assert(*r2 == 0x0101);     /*overflow and carry*/

   *r4 = 0x11;
   executeADD(0x11, r4, 1, r2);
   assert(*r4 == 0x22);
   assert(*r2 == 0);
   *r4 = 0x10FF;
   executeADD(0x10FF, r4, 1, r2);   /*minus minus*/
   assert(*r4 == 0xFE);
   assert(*r2 == 0x0005);
   *r4 = 0x02;
   executeADD(0xFF, r4, 1, r2);   /*positive minus*/
   assert(*r4 == 0x01);
   assert(*r2 == 0x0001);
   *r4 = 0xFD;
   executeADD(0x01, r4, 1, r2);   /* minus postive*/
   assert(*r4 == 0xFE);
   assert(*r2 == 0x0004);
   *r4 = 0xFF;
   executeADD(0x01, r4, 1, r2);
   assert(*r4 == 0x00);
   assert(*r2 == 0x03);
   *r4 = 0xFF;
   executeADD(0x00, r4, 1, r2);
   assert(*r4 == 0xFF);
   assert(*r2 == 0x04);
   *r4 = 0xFF;
   executeADD(0x02, r4, 1, r2);
   assert(*r4 == 0x01);
   assert(*r2 == 0x01);
   *r4 = 0x7F;
   executeADD(0x10, r4, 1, r2);
   assert(*r4 == 0x8F);
   assert(*r2 == 0x0104);
   *r4 = 0xFF;
   executeADD(0x80, r4, 1, r2);
   assert(*r4 == 0x7F);
   assert(*r2 == 0x0101);

   *r4 = 0x0004;
   *r2 = 0x0001;
   executeADDC(0x0004, r4, 0, r2);
   assert(*r4 == 0x0009);
   *r4 = 0xFFFF;
   *r2 = 0x0001;
   executeADDC(0xFFFF, r4, 0, r2);   /*minus minus*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0005);
   *r4 = 0x0002;
   *r2 = 0x0000;
   executeADDC(0xFFFF, r4, 0, r2);   /*positive minus*/
   assert(*r4 == 0x0001);
   assert(*r2 == 0x0001);
   *r4 = 0xFFFD;
   *r2 = 0x0001;
   executeADDC(0x0001, r4, 0, r2);   /* minus postive*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);
   *r4 = 0xFFFE;
   *r2 = 0x0000;
   executeADDC(0x0002, r4, 0, r2);   /*zero status*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);          /*zero bit plus carry bit*/
   *r4 = 0xFFFE;
   *r2 = 0;
   executeADDC(0x0001, r4, 0, r2);   /*negative status*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);
   *r4 = 0xFFFF;
   *r2 = 0;
   executeADDC(0x0002, r4, 0, r2);   /*carry status*/
   assert(*r4 == 0x0001);
   assert(*r2 == 0x0001);
   *r4 = 0x7FFF;
   *r2 = 0;
   executeADDC(0x1000, r4, 0, r2);   /*overflow status*/
   assert(*r4 == 0x8FFF);
   assert(*r2 == 0x0104);         /*overflow and negative*/
   *r4 = 0xFFFF;
   *r2 = 0;
   executeADDC(0x8000, r4, 0, r2);   /*overflow status*/
   assert(*r4 == 0x7FFF);
   assert(*r2 == 0x0101);     /*overflow and carry*/

   *r4 = 0x11;
   *r2 = 0x0001;
   executeADDC(0x11, r4, 1, r2);
   assert(*r4 == 0x23);
   assert(*r2 == 0);
   *r4 = 0x10FF;
   *r2 = 0x0001;
   executeADDC(0x10FF, r4, 1, r2);   /*minus minus*/
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0005);
   *r4 = 0x02;
   *r2 = 0x0000;
   executeADDC(0xFF, r4, 1, r2);   /*positive minus*/
   assert(*r4 == 0x01);
   assert(*r2 == 0x0001);
   *r4 = 0xFD;
   *r2 = 0x0001;
   executeADDC(0x01, r4, 1, r2);   /* minus postive*/
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);
   *r4 = 0xFF;
   *r2 = 0;
   executeADDC(0x01, r4, 1, r2);
   assert(*r4 == 0x00);
   assert(*r2 == 0x03);
   *r4 = 0xFF;
   *r2 = 0;
   executeADDC(0x00, r4, 1, r2);
   assert(*r4 == 0xFF);
   assert(*r2 == 0x04);
   *r4 = 0xFF;
   *r2 = 0;
   executeADDC(0x02, r4, 1, r2);
   assert(*r4 == 0x01);
   assert(*r2 == 0x01);
   *r4 = 0x7F;
   *r2 = 0;
   executeADDC(0x10, r4, 1, r2);
   assert(*r4 == 0x8F);
   assert(*r2 == 0x0104);
   *r4 = 0xFF;
   *r2 = 0;
   executeADDC(0x80, r4, 1, r2);
   assert(*r4 == 0x7F);
   assert(*r2 == 0x0101);

   *r4 = 0xFFFE;
   executeSUB(0xFFFF, r4, 0, r2);  /*minus  minus*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 4);
   *r4 = 0x0001;
   executeSUB(0xFFFF, r4, 0, r2);  /*postive  minus*/
   assert(*r4 == 0x0002);
   assert(*r2 == 0);
   *r4 = 0xFFFF;
   executeSUB(0x0001, r4, 0, r2);  /*  minus positive*/
   assert(*r4 == 0xFFFE);
   assert(*r2 == 5);
   *r4 = 0x0004;
   executeSUB(0x0003, r4, 0, r2);   /*carry*/
   assert(*r4 == 0x0001);
   assert(*r2 == 1);
   *r4 = 0x0004;
   executeSUB(0x0004, r4, 0, r2);
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0x0004;
   executeSUB(0x0005, r4, 0, r2);
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);    /* negative*/
   *r4 = 0xFFFE;
   executeSUB(0x07FFF, r4, 0, r2);   /*overflow + carry*/
   assert(*r4 == 0x7FFF);
   assert(*r2 == 0x0101);
   *r4 = 0x7FFF;
   executeSUB(0xFFFD, r4, 0, r2);   /*overflowS + negative*/
   assert(*r4 == 0x8002);
   assert(*r2 == 0x0104);

   *r4 = 0x10FE;
   executeSUB(0x10FF, r4, 1, r2);  /*minus  minus*/
   assert(*r4 == 0xFF);
   assert(*r2 == 4);
   *r4 = 0x01;
   executeSUB(0xFF, r4, 1, r2);  /*postive  minus*/
   assert(*r4 == 0x02);
   assert(*r2 == 0);
   *r4 = 0xFF;
   executeSUB(0x01, r4, 1, r2);  /*  minus positive*/
   assert(*r4 == 0xFE);
   assert(*r2 == 5);
   *r4 = 0x96;
   executeSUB(0x80, r4, 1, r2);
   assert(*r4 == 0x16);
   assert(*r2 == 1);
   *r4 = 0x04;
   executeSUB(0x04, r4, 1, r2);
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0x04;
   executeSUB(0x05, r4, 1, r2);
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);    /* negative*/
   *r4 = 0xFE;
   executeSUB(0x07F, r4, 1, r2);   /*overflow + carry*/
   assert(*r4 == 0x7F);
   assert(*r2 == 0x0101);
   *r4 = 0x7F;
   executeSUB(0xFD, r4, 1, r2);   /*overflowS + negative*/
   assert(*r4 == 0x82);
   assert(*r2 == 0x0104);

   *r4 = 0xFFFE;
   *r2 = 0;
   executeSUBC(0xFFFF, r4, 0, r2);  /*minus  minus*/
   assert(*r4 == 0xFFFE);
   assert(*r2 == 4);
   *r4 = 0x0001;
   *r2 = 1;
   executeSUBC(0xFFFF, r4, 0, r2);  /*postive  minus*/
   assert(*r4 == 0x0002);
   assert(*r2 == 0);
   *r4 = 0xFFFF;
   *r2 = 0;
   executeSUBC(0x0001, r4, 0, r2);  /*  minus positive*/
   assert(*r4 == 0xFFFD);
   assert(*r2 == 5);
   *r4 = 0x0004;
   *r2 = 1;
   executeSUBC(0x0003, r4, 0, r2);   /*carry*/
   assert(*r4 == 0x0001);
   assert(*r2 == 1);
   *r4 = 0x0004;
   *r2 = 0;
   executeSUBC(0x0003, r4, 0, r2);
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0x0004;
   *r2 = 1;
   executeSUBC(0x0005, r4, 0, r2);
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);    /* negative*/
   *r4 = 0xFFFE;
   *r2 = 1;
   executeSUBC(0x07FFF, r4, 0, r2);   /*overflow + carry*/
   assert(*r4 == 0x7FFF);
   assert(*r2 == 0x0101);
   *r4 = 0x7FFF;
   *r2 = 1;
   executeSUBC(0xFFFD, r4, 0, r2);   /*overflowS + negative*/
   assert(*r4 == 0x8002);
   assert(*r2 == 0x0104);

   *r4 = 0x10FE;
   *r2 = 0;
   executeSUBC(0x10FF, r4, 1, r2);  /*minus  minus*/
   assert(*r4 == 0xFE);
   assert(*r2 == 4);
   *r4 = 0x01;
   *r2 = 1;
   executeSUBC(0xFF, r4, 1, r2);  /*postive  minus*/
   assert(*r4 == 0x02);
   assert(*r2 == 0);
   *r4 = 0xFF;
   *r2 = 0;
   executeSUBC(0x01, r4, 1, r2);  /*  minus positive*/
   assert(*r4 == 0xFD);
   assert(*r2 == 5);
   *r4 = 0x96;
   *r2 = 1;
   executeSUBC(0x80, r4, 1, r2);
   assert(*r4 == 0x16);
   assert(*r2 == 1);
   *r4 = 0x04;
   *r2 = 0;
   executeSUBC(0x03, r4, 1, r2);
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0x04;
   *r2 = 1;
   executeSUBC(0x05, r4, 1, r2);
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);    /* negative*/
   *r4 = 0xFE;
   *r2 = 1;
   executeSUBC(0x07F, r4, 1, r2);   /*overflow + carry*/
   assert(*r4 == 0x7F);
   assert(*r2 == 0x0101);
   *r4 = 0x7F;
   *r2 = 1;
   executeSUBC(0xFD, r4, 1, r2);   /*overflowS + negative*/
   assert(*r4 == 0x82);
   assert(*r2 == 0x0104);

   *r4 = 0xFFFE;
   executeCMP(0xFFFF, r4, 0, r2);  /*minus  minus*/
   assert(*r4 == 0xFFFE);
   assert(*r2 == 4);
   *r4 = 0x0001;
   executeCMP(0xFFFF, r4, 0, r2);  /*postive  minus*/
   assert(*r4 == 0x0001);
   assert(*r2 == 0);
   *r4 = 0xFFFF;
   executeCMP(0x0001, r4, 0, r2);  /*  minus positive*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 5);
   *r4 = 0x0004;
   executeCMP(0x0003, r4, 0, r2);   /*carry*/
   assert(*r4 == 0x0004);
   assert(*r2 == 1);
   *r4 = 0x0004;
   executeCMP(0x0004, r4, 0, r2);
   assert(*r4 == 0x0004);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0x0004;
   executeCMP(0x0005, r4, 0, r2);
   assert(*r4 == 0x0004);
   assert(*r2 == 0x0004);    /* negative*/
   *r4 = 0xFFFE;
   executeCMP(0x07FFF, r4, 0, r2);   /*overflow + carry*/
   assert(*r4 == 0xFFFE);
   assert(*r2 == 0x0101);
   *r4 = 0x7FFF;
   executeCMP(0xFFFD, r4, 0, r2);   /*overflowS + negative*/
   assert(*r4 == 0x7FFF);
   assert(*r2 == 0x0104);

   *r4 = 0x10FE;
   executeCMP(0x10FF, r4, 1, r2);  /*minus  minus*/
   assert(*r4 == 0x10FE);
   assert(*r2 == 4);
   *r4 = 0x01;
   executeCMP(0xFF, r4, 1, r2);  /*postive  minus*/
   assert(*r4 == 0x01);
   assert(*r2 == 0);
   *r4 = 0xFF;
   executeCMP(0x01, r4, 1, r2);  /*  minus positive*/
   assert(*r4 == 0xFF);
   assert(*r2 == 5);
   *r4 = 0x96;
   executeCMP(0x80, r4, 1, r2);
   assert(*r4 == 0x96);
   assert(*r2 == 1);
   *r4 = 0x04;
   executeCMP(0x04, r4, 1, r2);
   assert(*r4 == 0x04);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0x04;
   executeCMP(0x05, r4, 1, r2);
   assert(*r4 == 0x04);
   assert(*r2 == 0x0004);    /* negative*/
   *r4 = 0xFE;
   executeCMP(0x07F, r4, 1, r2);   /*overflow + carry*/
   assert(*r4 == 0xFE);
   assert(*r2 == 0x0101);
   *r4 = 0x7F;
   executeCMP(0xFD, r4, 1, r2);   /*overflowS + negative*/
   assert(*r4 == 0x7F);
   assert(*r2 == 0x0104);

   *r4 = 0x0004;
   *r2 = 0x0001;
   executeDADD(0x0004, r4, 0, r2);
   assert(*r4 == 0x0009);
   assert(*r2 == 0);
   *r4 = 0x0004;
   *r2 = 0x0000;
   executeDADD(0x0004, r4, 0, r2);
   assert(*r4 == 0x0008);
   assert(*r2 == 0);
   *r4 = 0x0004;
   *r2 = 0x0001;
   executeDADD(0x0009, r4, 0, r2);
   assert(*r4 == 0x0014);
   assert(*r2 == 0);
   *r4 = 0x5555;
   *r2 = 0x00001;
   executeDADD(0x8888, r4, 0, r2);
   assert(*r4 == 0x4444);
   assert(*r2 == 1);
   *r4 = 0x4444;                 /*bcd  representation only postive integets*/
   *r2 = 0x0001;
   executeDADD(0x5555, r4, 0, r2); /* carry */
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);
   *r4 = 0x8000;
   *r2 = 0x0000;
   executeDADD(0, r4, 0, r2); /* negative */
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0004);
   *r4 = 0x0;
   *r2 = 0x0000;
   executeDADD(0, r4, 0, r2); /* zero */
   assert(*r4 == 0);
   assert(*r2 == 0x0002);

   *r4 = 0x1004;
   *r2 = 0x01;
   executeDADD(0x1004, r4, 1, r2);
   assert(*r4 == 0x09);
   assert(*r2 == 0);
   *r4 = 0x04;
   *r2 = 0x00;
   executeDADD(0x04, r4, 1, r2);
   assert(*r4 == 0x08);
   assert(*r2 == 0);
   *r4 = 0x04;
   *r2 = 0x0001;
   executeDADD(0x09, r4, 1, r2);
   assert(*r4 == 0x14);
   assert(*r2 == 0);
   *r4 = 0x55;
   *r2 = 0x00001;
   executeDADD(0x88, r4, 1, r2);
   assert(*r4 == 0x44);
   assert(*r2 == 1);
   *r4 = 0x99;                 /*bcd  representation only postive integets*/
   *r2 = 0x0000;
   executeDADD(0x01, r4, 1, r2); /* carry */
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);
   *r4 = 0x80;
   *r2 = 0x00;
   executeDADD(0, r4, 1, r2); /* negative + carry */
   assert(*r4 == 0x80);
   assert(*r2 == 0x0004);
   *r4 = 0x0;
   *r2 = 0x0000;
   executeDADD(0, r4, 1, r2); /* zero */
   assert(*r4 == 0);
   assert(*r2 == 0x0002);

   *r4 = 0x000C;
   executeAND(0x000A, r4, 0, r2);    /* carry*/
   assert(*r4 == 0x0008);
   assert(*r2 == 0x0001);
   *r4 = 0x0001;
   executeAND(0x0000, r4, 0, r2);   /*zero*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0002);
   *r4 = 0x8001;
   executeAND(0x8000, r4, 0, r2);   /*negative and carry*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0005);
   *r4 = 0x100C;
   executeAND(0x100A, r4, 1, r2);
   assert(*r4 == 0x0008);
   assert(*r2 == 0x0001);
   *r4 = 0x01;
   executeAND(0x00, r4, 1, r2);   /*zero*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0002);
   *r4 = 0x81;
   executeAND(0x80, r4, 1, r2);   /*negative*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0005);

   *r4 = 0x000C;
   executeBIT(0x000A, r4, 0, r2);    /* carry*/
   assert(*r4 == 0x000C);
   assert(*r2 == 0x0001);
   *r4 = 0x0001;
   executeBIT(0x0000, r4, 0, r2);   /*zero*/
   assert(*r4 == 0x0001);
   assert(*r2 == 0x0002);
   *r4 = 0x8001;
   executeBIT(0x8000, r4, 0, r2);   /*negative and carry*/
   assert(*r4 == 0x8001);
   assert(*r2 == 0x0005);
   *r4 = 0x100C;
   executeBIT(0x100A, r4, 1, r2);
   assert(*r4 == 0x100C);
   assert(*r2 == 0x0001);
   *r4 = 0x01;
   executeBIT(0x00, r4, 1, r2);   /*zero*/
   assert(*r4 == 0x01);
   assert(*r2 == 0x0002);
   *r4 = 0x81;
   executeBIT(0x80, r4, 1, r2);   /*negative*/
   assert(*r4 == 0x81);
   assert(*r2 == 0x0005);

   *r4 = 0x000C;
   executeBIC(0x000A, r4, 0);
   assert(*r4 == 0x0004);
   *r4 = 0x100C;
   executeBIC(0x100A, r4, 1);
   assert(*r4 == 0x0004);

   *r4 = 0x000C;
   executeBIS(0x000A, r4, 0);
   assert(*r4 == 0x000E);
   *r4 = 0x100C;
   executeBIS(0x100A, r4, 1);
   assert(*r4 == 0x000E);

   *r4 = 0x000C;
   executeXOR(0x000A, r4, 0, r2);  /*carry*/
   assert(*r4 == 0x0006);
   assert(*r2 == 0x0001);
   *r4 = 0x0001;
   executeXOR(0x0001, r4, 0, r2); /* zero*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0002);
   *r4 = 0x8001;
   executeXOR(0x0001, r4, 0, r2); /* negative and carry*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0005);
   *r4 = 0x100C;
   executeXOR(0x100A, r4, 1, r2);
   assert(*r4 == 0x0006);
   assert(*r2 == 0x0001);
   *r4 = 0x01;
   executeXOR(0x01, r4, 1, r2); /* zero*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0002);
   *r4 = 0x81;
   executeXOR(0x01, r4, 1, r2); /* negative and carry*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0005);

   *r4 = 0x0004;
   formatOneExecute(0x4405 ,ram->memorySpace ,r->registerArray, 4);
   assert(r->registerArray[5] == 0x0004);
   assert(*r0 == 0x0002);

   *r4 = 0x0004;
   formatOneExecute(0x5405 ,ram->memorySpace ,r->registerArray, 5);
   assert(r->registerArray[5] == 0x0008);
   assert(*r0 == 0x0004);

   *r4 = 0x0004;
   r->registerArray[5] = 0x0004;
   *r2 = 0x0001;
   formatOneExecute(0x6405 ,ram->memorySpace ,r->registerArray, 6);
   assert(r->registerArray[5] == 0x0009);
   assert(*r0 == 0x0006);

   *r4 = 0x0004;
   r->registerArray[5] = 0x0004;
   *r2 = 0x0000;
   formatOneExecute(0x7405 ,ram->memorySpace ,r->registerArray, 7);
   assert(r->registerArray[5] == 0xFFFF);
   assert(*r0 == 0x0008);

   *r4 = 0x0004;
   r->registerArray[5] = 0x0004;
   formatOneExecute(0x8405 ,ram->memorySpace ,r->registerArray, 8);
   assert(r->registerArray[5] == 0x0000);
   assert(*r0 == 0x000A);

   *r4 = 0x0004;
   r->registerArray[5] = 0x0004;
   formatOneExecute(0x9405 ,ram->memorySpace ,r->registerArray, 9);
   assert(r->registerArray[5] == 0x0004);
   assert(*r0 == 0x000C);

   *r4 = 0x1388;
   r->registerArray[5] = 0x1388;
   *r2 = 0x0001;
   formatOneExecute(0xA405 ,ram->memorySpace ,r->registerArray, 10);
   assert(r->registerArray[5] == 0x2777);
   assert(*r2 == 0x0000);
   assert(*r0 == 0x000E);

   *r4 = 0x000A;
   r->registerArray[5] = 0x000C;
   formatOneExecute(0xB405 ,ram->memorySpace ,r->registerArray, 11);
   assert(r->registerArray[5] == 0x000C);
   assert(*r2 == 0x0001);
   assert(*r0 == 0x0010);

   *r4 = 0x000A;
   r->registerArray[5] = 0x000C;
   formatOneExecute(0xC405 ,ram->memorySpace ,r->registerArray, 12);
   assert(r->registerArray[5] == 0x0004);
   assert(*r0 == 0x0012);

   *r4 = 0x000A;
   r->registerArray[5] = 0x000C;
   formatOneExecute(0xD405 ,ram->memorySpace ,r->registerArray, 13);
   assert(r->registerArray[5] == 0x000E);
   assert(*r0 == 0x0014);

   *r4 = 0x000A;
   r->registerArray[5] = 0x000C;
   *r2 = 0x0001;
   formatOneExecute(0xE405 ,ram->memorySpace ,r->registerArray, 14);
   assert(r->registerArray[5] == 0x0006);
   assert(*r2 = 0x0001);
   assert(*r0 == 0x0016);

   *r4 = 0x000A;
   r->registerArray[5] = 0x000C;
   *r2 = 0x0001;
   formatOneExecute(0xF405 ,ram->memorySpace ,r->registerArray, 15);
   assert(r->registerArray[5] == 0x0008);
   assert(*r2 = 0x0001);
   assert(*r0 == 0x0018);
}

void testDADDFunctions(){
   int i;
   uint8_t *srcDigits = (uint8_t*)malloc(4*sizeof(uint8_t)), *dstDigits = (uint8_t*)malloc(4*sizeof(uint8_t));
   bool *isCarry = (bool*)malloc(sizeof(bool));
   uint16_t result = 0;
   srcDigits = getDigits(srcDigits, 0x3210, 0);
   for(i = 0; i < 4; i++){
      assert(srcDigits[i] == i);
   }
   srcDigits = getDigits(srcDigits, 0x3210, 1);
   for(i = 0; i < 2; i++){
      assert(srcDigits[i] == i);
   }
   srcDigits = getDigits(srcDigits, 0x4567, 0);
   dstDigits = getDigits(dstDigits, 0x4567, 0);
   assert(daddAddition(srcDigits, dstDigits, 0, isCarry, 0) == 0x9134);
   assert(*isCarry == false);
   srcDigits = getDigits(srcDigits, 0x5555, 0);
   dstDigits = getDigits(dstDigits, 0x7777, 0);
   assert(daddAddition(srcDigits, dstDigits, 0, isCarry, 0) == 0x3332);
   assert(*isCarry == true);
   srcDigits = getDigits(srcDigits, 0x4567, 0);
   dstDigits = getDigits(dstDigits, 0x4567, 0);
   assert(daddAddition(srcDigits, dstDigits, 1, isCarry, 0) == 0x9135);
   assert(*isCarry == false);
   srcDigits = getDigits(srcDigits, 0x5555, 0);
   dstDigits = getDigits(dstDigits, 0x7777, 0);
   assert(daddAddition(srcDigits, dstDigits, 1, isCarry, 0) == 0x3333);
   assert(*isCarry == true);

   srcDigits = getDigits(srcDigits, 0x1045, 0);
   dstDigits = getDigits(dstDigits, 0x1045, 0);
   assert(daddAddition(srcDigits, dstDigits, 0, isCarry, 1) == 0x90);
   assert(*isCarry == false);
   srcDigits = getDigits(srcDigits, 0x55, 0);
   dstDigits = getDigits(dstDigits, 0x77, 0);
   assert(daddAddition(srcDigits, dstDigits, 0, isCarry, 1) == 0x32);
   assert(*isCarry == true);
   srcDigits = getDigits(srcDigits, 0x45, 0);
   dstDigits = getDigits(dstDigits, 0x45, 0);
   assert(daddAddition(srcDigits, dstDigits, 1, isCarry, 1) == 0x91);
   assert(*isCarry == false);
   srcDigits = getDigits(srcDigits, 0x55, 0);
   dstDigits = getDigits(dstDigits, 0x77, 0);
   assert(daddAddition(srcDigits, dstDigits, 1, isCarry, 1) == 0x33);
   assert(*isCarry == true);

   result = addFourBitsToResult(result, 9, 0);
   result = addFourBitsToResult(result, 7, 4);
   result = addFourBitsToResult(result, 5, 8);
   result = addFourBitsToResult(result, 3, 12);
   assert(result == 0x3579);
   result = 0;
   result = addFourBitsToResult(result, 9, 0);
   result = addFourBitsToResult(result, 7, 4);
   assert(result == 0x79);
   free(srcDigits);
   free(dstDigits);
   free(isCarry);
}

void testflagHandler(){
   registers *r = registerInit();
   uint16_t *r2 = &r->registerArray[2];
   changeZeroBit(0 ,r2);
   assert(*r2 == 2);
   changeZeroBit(1 ,r2);
   assert(*r2 == 0);
   changeNegativeBit(0xFFFF, r2, 0);
   assert(*r2 == 4);
   changeNegativeBit(0x0001, r2, 0);
   assert(*r2 == 0);
   changeNegativeBit(0xFF, r2, 1);
   assert(*r2 == 4);
   changeNegativeBit(0x0001, r2, 1);
   assert(*r2 == 0);
   changeCarryBit(0xFFFF, 0x0001, 0, r2, 0, WORDCARRY, BYTECARRY);
   assert(*r2 == 1);
   changeCarryBit(0xFFFF, 0,0, r2, 0, WORDCARRY, BYTECARRY);
   assert(*r2 == 0);
   changeCarryBit(0xFF, 0x01,0, r2, 1, WORDCARRY, BYTECARRY);
   assert(*r2 == 1);
   changeCarryBit(0xFF, 0x00,0, r2, 1, WORDCARRY, BYTECARRY);
   assert(*r2 == 0);
   changeCarryForBitOp(0, r2);
   assert(*r2 == 0);
   changeCarryForBitOp(1, r2);
   assert(*r2 == 1);
   *r2 = 0;
   changeOverflowBit(0x9C40, r2, 0, 0x4E20, 0x4E20, true);
   assert(*r2 == 256);
   changeOverflowBit(0x0002, r2, 0, 0x0001, 0x0001, true);
   assert(*r2 == 0);
   changeOverflowBit(0xF0, r2, 1, 0x78, 0x78, true);
   assert(*r2 == 256);
   changeOverflowBit(0x0002, r2, 1, 0x0001, 0x0001, true);
   assert(*r2 == 0);

   assert(isNegative(0x0001, 0) == false);
   assert(isNegative(0xF001, 0) == true);
   assert(isNegative(0x0001, 1) == false);
   assert(isNegative(0x00F1, 1) == true);

   assert(isCarry(0x0001, 0x0001, 0, 0, WORDCARRY, BYTECARRY) == false);
   assert(isCarry(0xFFFF, 0x0001, 0, 0, WORDCARRY, BYTECARRY) == true);
   assert(isCarry(0x01, 0x01, 0, 1, WORDCARRY, BYTECARRY) == false);
   assert(isCarry(0xFF, 0x01, 0,1, WORDCARRY, BYTECARRY) == true);

   assert(isCarry(0x0001, 0x0001, 1, 0, WORDCARRY, BYTECARRY) == false);
   assert(isCarry(0xFFFF, 0x0000, 1, 0, WORDCARRY, BYTECARRY) == true);
   assert(isCarry(0x01, 0x00, 1, 1, WORDCARRY, BYTECARRY) == false);
   assert(isCarry(0xFF, 0x00, 1,1, WORDCARRY, BYTECARRY) == true);

   assert(isOverflow(0x9C40, 0, 0x4E20, 0x4E20, true) == true);
   assert(isOverflow(0x0002, 0, 0x0001, 0x0001, true) == false);
   assert(isOverflow(0x10, 1, 0x88, 0x88, true) == true);
   assert(isOverflow(0x0002, 1, 0x0001, 0x0001, true) == false);

   assert(isOverflow(0x9C40, 0, 0xB1E0, 0x4E20, false) == true);
   assert(isOverflow(0x0001, 0, 0x0001, 0x0002, false) == false);
   assert(isOverflow(0x10, 1, 0x78, 0x88, false) == true);
   assert(isOverflow(0x01, 1, 0x01, 0x02, false) == false);

   *r2 = 0x0100;
   resetOverflowBit(r2);
   assert(*r2 == 0);

   assert(getMsb(0xFFFF, 0) == 1);
   assert(getMsb(0x00FF, 1) == 1);
}

void testdecode(){
   memory *ram;
   registers *r;
   uint16_t *r0, src, *dst, *r4;
   ram = memoryInit();
   r = registerInit();
   r0 = &r->registerArray[0];
   r4 = &r->registerArray[4];

   r->registerArray[4] = 0x0002;
   assert(getSrc(0x4404 , ram->memorySpace, r->registerArray, 1) == 0x0002);
   assert(getSrc(0x4304 , ram->memorySpace, r->registerArray, 1) == 0x0000);

   *r0 = 0x0000;
   r->registerArray[4] = 0x0000;
   ram->memorySpace[2] = 0x0004;
   ram->memorySpace[4] = 0x0005;
   src = getSrc(0x4414 , ram->memorySpace, r->registerArray, 1);
   assert(src == 0x0005);
   assert(*r0 == 0x0002);
   assert(getSrc(0x4314 , ram->memorySpace, r->registerArray, 1) == 0x0001);

   *r0 = 0x0000;
   ram->memorySpace[2] = 0x0002;
   ram->memorySpace[4] = 0x0005;
   src = getSrc(0x4014 , ram->memorySpace, r->registerArray, 1);
   assert(src == 0x0005);
   assert(*r0 == 0x0002);

   *r0 = 0x0000;
   ram->memorySpace[2] = 0x0004;
   ram->memorySpace[4] = 0x0005;
   src = getSrc(0x4214 , ram->memorySpace, r->registerArray, 1);
   assert(src == 0x0005);
   assert(*r0 == 0x0002);

   r->registerArray[4] = 0x0002;
   ram->memorySpace[2] = 0x0007;
   assert(getSrc(0x4424 , ram->memorySpace, r->registerArray, 1) == 0x0007);
   assert(getSrc(0x4324 , ram->memorySpace, r->registerArray, 1) == 0x0002);
   assert(getSrc(0x4224 , ram->memorySpace, r->registerArray, 1) == 0x0004);

   r->registerArray[4] = 0x0002;
   ram->memorySpace[2] = 0x0004;
   src = getSrc(0x4434 , ram->memorySpace, r->registerArray, 1);
   assert(src == 0x0004);
   assert(r->registerArray[4] == 0x0004);
   assert(*r0 == 0x0002);

   r->registerArray[4] = 0x0002;
   ram->memorySpace[2] = 0x0004;
   src = getSrc(0x4474 , ram->memorySpace, r->registerArray, 1);
   assert(src == 0x0004);
   assert(r->registerArray[4] == 0x0003);
   assert(*r0 == 0x0002);

   *r0 = 0x0000;
   ram->memorySpace[2] = 0x0007;
   assert(getSrc(0x4034 , ram->memorySpace, r->registerArray, 1) == 0x0007);
   /*register mode*/
   assert(getDst(0x4404 , ram->memorySpace, r->registerArray) == &r->registerArray[4]);
   /*index mode*/
   *r0 = 0x0000;
   r->registerArray[4] = 0x0000;
   ram->memorySpace[2] = 0x0004;
   ram->memorySpace[4] = 0x0005;
   dst = getDst(0x4484 , ram->memorySpace, r->registerArray);
   assert(dst == &ram->memorySpace[4]);
   assert(*r0 == 0x0002);
   /*symbolic mode*/
   *r0 = 0x0000;
   ram->memorySpace[2] = 0x0002;
   ram->memorySpace[4] = 0x0005;
   dst = getDst(0x4480 , ram->memorySpace, r->registerArray);
   assert(dst == &ram->memorySpace[4]);
   assert(*r0 == 0x0002);
   /*absolute mode*/
   *r0 = 0x0000;
   ram->memorySpace[2] = 0x0004;
   ram->memorySpace[4] = 0x0005;
   dst = getDst(0x4482 , ram->memorySpace, r->registerArray);
   assert(dst == &ram->memorySpace[4]);
   assert(*r0 == 0x0002);

   r->registerArray[4] = 0x0000;
   *r0 = 0x0000;
   ram->memorySpace[2] = 0x0004;
   *dst = decodeCall(0x1294 ,r->registerArray, ram->memorySpace);
   assert(*dst == 0x0004);
   assert(*r0 == 0x0002);

   *r0 = 0x0002;
   ram->memorySpace[4] = 0x0006;
   *dst = decodeCall(0x1290 ,r->registerArray, ram->memorySpace);
   assert(*dst == 0x000a);
   assert(*r0 == 0x0004);

   *r0 = 0x0000;
   ram->memorySpace[2] = 0x0006;
   *dst = decodeCall(0x1292 ,r->registerArray, ram->memorySpace);
   assert(*dst == 0x0006);
   assert(*r0 == 0x0002);

   *r0 = 0x0000;
   ram->memorySpace[2] = 0x0006;
   *dst = decodeCall(0x12B0 ,r->registerArray, ram->memorySpace);
   assert(*dst == 0x0006);
   assert(*r0 == 0x0002);

   *r0 = 0x0000;
   *r4 = 0x0004;
   *dst = decodeCall(0x1284 ,r->registerArray, ram->memorySpace);
   assert(*dst == 0x0004);
   assert(*r0 == 0x0000);

   *r0 = 0x0000;
   *r4 = 0x0004;
   ram->memorySpace[4] = 0x0006;
   *dst = decodeCall(0x12A4 ,r->registerArray, ram->memorySpace);
   assert(*dst == 0x0006);
   assert(*r0 == 0x0000);

   *r0 = 0x0000;
   *r4 = 0x0004;
   ram->memorySpace[4] = 0x0006;
   *dst = decodeCall(0x12B4 ,r->registerArray, ram->memorySpace);
   assert(*dst == 0x0006);
   assert(*r0 == 0x0000);
   assert(*r4 == 0x0006);

   assert(getSreg(0x4404, 1) == 4);
   assert(getSreg(0x1004, 2) == 4);

   assert(getDreg(0x1004) == 4);

   assert(byteOrWord(0x4444) == 1);
   assert(byteOrWord(0x4404) == 0);

   assert(getDstAddressMode(0x4484) == 1);
   assert(getDstAddressMode(0x4404) == 0);

   assert(getOffset(0x2000) == 0);
   assert(getOffset(0x203F) == 63);
   assert(getOffset(0x3ff3) == -13);

}
void testemulator(){
   memory *ram;
   registers *r;
   emulation emulator;
   ram = memoryInit();
   r = registerInit();
   emulator.ram = ram;
   emulator.r = r;
   emulator.ram->memorySpace[0] = 0x4405; /*MOV r4, r5*/
   emulator.r->registerArray[4] = 0x0001;
   emulatorExec(emulator);
   assert(emulator.r->registerArray[5] == 0x0001);
   assert(emulator.r->registerArray[0] == 0x0002);

   emulator.r->registerArray[2] = 0x0000;
   assert(isCPUOFF(emulator) == false);
   emulator.r->registerArray[2] = 0x0010;
   assert(isCPUOFF(emulator) == true);

   assert(calc2sComplement(0xFFFF) == -1);
   assert(calcByte2sComplement(0x1FF) == -1);
   assert(calcBCD(0x91) == 91);
}

void testinstructions(){
   memory *ram;
   registers *r;
   int i;
   ram = memoryInit();
   r = registerInit();

   for(i = 0; i < 12; i++){
     assert(r->registerArray[i] == 0);
   }
   r->registerArray[4] = 0x0002;
   instructionsExecute(0x4405, ram, r);
   assert(r->registerArray[4] = 0x0002);
   assert(r->registerArray[0] == 0x0002);

   r->registerArray[4] = 0x0F0F;
   instructionsExecute(0x1004, ram, r);
   assert(r->registerArray[4] == 0x0787);
   assert(r->registerArray[0] == 0x0004);

   instructionsExecute(0x3C04, ram, r);
   assert(r->registerArray[0] == 0x000E);
}

void testEmulatedInstructions(){
   memory *ram;
   registers *r;
   uint16_t *r0, *r1, *r2, *r4;
   ram = memoryInit();
   r = registerInit();

   r4 = &r->registerArray[4];
   r2 = &r->registerArray[2];
   r0 = &r->registerArray[0];
   r1 = &r->registerArray[1];

   instructionsExecute(0x6304, ram, r);  /*adc*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0002);    /*zero*/
   *r2 = 1;
   instructionsExecute(0x6304, ram, r);
   assert(*r4 == 0x0001);
   assert(*r2 == 0x0000);
   *r4 = 0xFFFF;
   *r2 = 1;
   instructionsExecute(0x6304, ram, r); /*carry and zero*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);
   *r4 = 0xFFFE;
   *r2 = 1;
   instructionsExecute(0x6304, ram, r); /*negative*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);
   *r4 = 0x7FFF;
   *r2 = 1;
   instructionsExecute(0x6304, ram, r); /*overflow and negative*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0104);

   instructionsExecute(0x6344, ram, r);  /*adc*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0002);    /*zero*/
   *r2 = 1;
   instructionsExecute(0x6344, ram, r);
   assert(*r4 == 0x01);
   assert(*r2 == 0x0000);
   *r4 = 0x10FF;
   *r2 = 1;
   instructionsExecute(0x6344, ram, r); /*carry and zero*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);
   *r4 = 0xFE;
   *r2 = 1;
   instructionsExecute(0x6344, ram, r); /*negative*/
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);
   *r4 = 0x7F;
   *r2 = 1;
   instructionsExecute(0x6344, ram, r); /*overflow and negative*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0104);

   *r4 = 0x0004;
   instructionsExecute(0x4400, ram, r);   /*branch*/
   assert(*r0 == 0x0006);

   instructionsExecute(0x4304, ram, r);  /*clr*/
   assert(*r4== 0x0000);

   instructionsExecute(0x4302, ram, r);
   assert(*r2 == 0x0000);

   instructionsExecute(0xd312, ram, r);  /*setc*/
   assert(*r2 == 1);
   instructionsExecute(0xc312, ram, r);  /*clrc*/
   assert(*r2 == 0);

   instructionsExecute(0xd222, ram, r);  /*setn*/
   assert(*r2== 4);
   instructionsExecute(0xc222, ram, r);  /*clrn*/
   assert(*r2 == 0);

   instructionsExecute(0xd322, ram, r);  /*setz*/
   assert(*r2 == 2);
   instructionsExecute(0xc322, ram, r);  /*clrz*/
   assert(*r2 == 0);

   instructionsExecute(0xd232, ram, r);  /*EINT*/
   assert(*r2 == 8);
   instructionsExecute(0xc232, ram, r);  /*DINT*/
   assert(*r2 == 0);


   instructionsExecute(0x5314, ram, r);  /*INC*/
   assert(*r4 == 0x0001);
   assert(*r2 == 0x0000);
   *r4 = 0xFFFF;
   instructionsExecute(0x5314, ram, r); /*carry and zero*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);
   *r4 = 0xFFFE;
   instructionsExecute(0x5314, ram, r); /*negative*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);
   *r4 = 0x7FFF;
   instructionsExecute(0x5314, ram, r); /*overflow and negative*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0104);

   *r4 = 0;
   instructionsExecute(0x5354, ram, r);
   assert(*r4 == 0x01);
   assert(*r2 == 0x0000);
   *r4 = 0x10FF;
   *r2 = 1;
   instructionsExecute(0x5354, ram, r); /*carry and zero*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);
   *r4 = 0xFE;
   *r2 = 1;
   instructionsExecute(0x5354, ram, r); /*negative*/
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);
   *r4 = 0x7F;
   *r2 = 1;
   instructionsExecute(0x5354, ram, r); /*overflow and negative*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0104);

   *r4 = 0;
   instructionsExecute(0x5324, ram, r);  /*INCd*/
   assert(*r4 == 0x0002);
   assert(*r2 == 0x0000);
   *r4 = 0xFFFE;
   instructionsExecute(0x5324, ram, r); /*carry and zero*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);
   *r4 = 0xFFFD;
   instructionsExecute(0x5324, ram, r); /*negative*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);
   *r4 = 0x7FFE;
   instructionsExecute(0x5324, ram, r); /*overflow and negative*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0104);

   *r4 = 0;
   instructionsExecute(0x5364, ram, r);
   assert(*r4 == 0x02);
   assert(*r2 == 0x0000);
   *r4 = 0x10FE;
   *r2 = 1;
   instructionsExecute(0x5364, ram, r); /*carry and zero*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);
   *r4 = 0xFD;
   *r2 = 1;
   instructionsExecute(0x5364, ram, r); /*negative*/
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);
   *r4 = 0x7E;
   *r2 = 1;
   instructionsExecute(0x5364, ram, r); /*overflow and negative*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0104);

   *r4 = 2;
   instructionsExecute(0x8314, ram, r);  /*dec*/
   assert(*r4 == 1);
   assert(*r2 == 0x0001);                /*carry*/
   *r4 = 1;
   instructionsExecute(0x8314, ram, r);  /*zero and carry*/
   assert(*r4 == 0);
   assert(*r2 == 0x0003);
   *r4 = 0;
   instructionsExecute(0x8314, ram, r);  /*negative*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);
   *r4 = 0x8000;
   instructionsExecute(0x8314, ram, r);  /*overflow + carry*/
   assert(*r4 == 0x7FFF);
   assert(*r2 == 0x0101);

   *r4 = 0x1002;
   instructionsExecute(0x8354, ram, r);
   assert(*r4 == 0x01);
   assert(*r2 == 0x0001);                /*carry*/
   *r4 = 0x01;
   instructionsExecute(0x8354, ram, r);  /*zero and carry*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);
   *r4 = 0x00;
   instructionsExecute(0x8354, ram, r);  /*negative*/
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);
   *r4 = 0x80;
   instructionsExecute(0x8354, ram, r);  /*overflow + carry*/
   assert(*r4 == 0x7F);
   assert(*r2 == 0x0101);

   *r4 = 3;
   instructionsExecute(0x8324, ram, r);  /*decd*/
   assert(*r4 == 1);
   assert(*r2 == 0x0001);                /*carry*/
   *r4 = 2;
   instructionsExecute(0x8324, ram, r);  /*zero and carry*/
   assert(*r4 == 0);
   assert(*r2 == 0x0003);
   *r4 = 1;
   instructionsExecute(0x8324, ram, r);  /*negative*/
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);
   *r4 = 0x8001;
   instructionsExecute(0x8324, ram, r);  /*overflow + carry*/
   assert(*r4 == 0x7FFF);
   assert(*r2 == 0x0101);

   *r4 = 0x1003;
   instructionsExecute(0x8364, ram, r);
   assert(*r4 == 0x01);
   assert(*r2 == 0x0001);                /*carry*/
   *r4 = 0x02;
   instructionsExecute(0x8364, ram, r);  /*zero and carry*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);
   *r4 = 0x01;
   instructionsExecute(0x8364, ram, r);  /*negative*/
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);
   *r4 = 0x81;
   instructionsExecute(0x8364, ram, r);  /*overflow + carry*/
   assert(*r4 == 0x7F);
   assert(*r2 == 0x0101);

   *r4 = 0x0004;
   *r2 = 0x0001;
   instructionsExecute(0xa304, ram, r);  /*dadc*/
   assert(*r4 == 0x0005);
   assert(*r2 == 0);
   *r4 = 0x0004;
   *r2 = 0x0000;
   instructionsExecute(0xa304, ram, r);
   assert(*r4 == 0x0004);
   assert(*r2 == 0);
   *r4 = 0x0009;
   *r2 = 0x0001;
   instructionsExecute(0xa304, ram, r);
   assert(*r4 == 0x0010);
   assert(*r2 == 0);
   *r4 = 0x9999;                 /*bcd  representation only postive integets*/
   *r2 = 0x0001;
   instructionsExecute(0xa304, ram, r); /* carry */
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);
   *r4 = 0x8000;
   *r2 = 0x0000;
   instructionsExecute(0xa304, ram, r); /* negative + carry */
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0004);
   *r4 = 0x0;
   *r2 = 0x0000;
   instructionsExecute(0xa304, ram, r); /* zero */
   assert(*r4 == 0);
   assert(*r2 == 0x0002);

   *r4 = 0x04;
   *r2 = 0x0000;
   instructionsExecute(0xa344, ram, r);
   assert(*r4 == 0x04);
   *r4 = 0x04;
   *r2 = 0x01;
   instructionsExecute(0xa344, ram, r);
   assert(*r4 == 0x05);
   assert(*r2 == 0);
   *r4 = 0x09;
   *r2 = 0x0001;
   instructionsExecute(0xa304, ram, r);
   assert(*r4 == 0x10);
   assert(*r2 == 0);
   *r4 = 0x99;                 /*bcd  representation only postive integets*/
   *r2 = 0x0001;
   instructionsExecute(0xa344, ram, r); /* carry */
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);
   *r4 = 0x80;
   *r2 = 0x00;
   instructionsExecute(0xa344, ram, r);/* negative + carry */
   assert(*r4 == 0x80);
   assert(*r2 == 0x0004);
   *r4 = 0x0;
   *r2 = 0x0000;
   instructionsExecute(0xa344, ram, r); /* zero */
   assert(*r4 == 0);
   assert(*r2 == 0x0002);


   *r4 = 0xFFFC;
   instructionsExecute(0xe334, ram, r); /*inv*/   /*carry*/
   assert(*r4 == 0x0003);
   assert(*r2 == 0x0001);
   *r4 = 0xFFFF;
   instructionsExecute(0xe334, ram, r); /* zero*/
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0002);
   *r4 = 0x7FFF;
   instructionsExecute(0xe334, ram, r); /* negative and carry*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0005);
   *r4 = 0x10FC;
   instructionsExecute(0xe374, ram, r);
   assert(*r4 == 0x03);
   assert(*r2 == 0x0001);
   *r4 = 0xFF;
   instructionsExecute(0xe374, ram, r); /* zero*/
   assert(*r4 == 0x00);
   assert(*r2 == 0x0002);
   *r4 = 0x7F;
   instructionsExecute(0xe374, ram, r);/* negative and carry*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0005);

   instructionsExecute(0x4303, ram, r);  /*nop*/

   r->registerArray[1] = 0x000a;
   ram->memorySpace[10] = 0x0001;
   instructionsExecute(0x4134, ram, r);  /*pop*/
   assert(*r4 == 1);

   *r4 = 0x0004;
   *r2 = 0x0001;
   instructionsExecute(0x6404, ram, r);  /*rlc*/
   assert(*r4 == 0x0009);
   assert(*r2 == 0x0000);
   *r4 = 0x0000;
   *r2 = 0x0000;
   instructionsExecute(0x6404, ram, r);
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0002);          /*zero bit*/
   *r4 = 0xC000;
   *r2 = 0;
   instructionsExecute(0x6404, ram, r);   /*negative + carry*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0005);
   *r4 = 0x8001;
   *r2 = 0;
   instructionsExecute(0x6404, ram, r);   /*carry status + overflow*/
   assert(*r4 == 0x0002);
   assert(*r2 == 0x0101);
   *r4 = 0x4000;
   *r2 = 0;
   instructionsExecute(0x6404, ram, r);   /*overflow status*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0104);         /*overflow and negative*/

   *r4 = 0x04;
   *r2 = 0x0001;
   instructionsExecute(0x6444, ram, r);  /*rlc*/
   assert(*r4 == 0x09);
   assert(*r2 == 0x0000);
   *r4 = 0x00;
   *r2 = 0x0000;
   instructionsExecute(0x6444, ram, r);
   assert(*r4 == 0x00);
   assert(*r2 == 0x0002);          /*zero bit*/
   *r4 = 0xC0;
   *r2 = 0;
   instructionsExecute(0x6444, ram, r);   /*negative + carry*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0005);
   *r4 = 0x81;
   *r2 = 0;
   instructionsExecute(0x6444, ram, r);   /*carry status + overflow*/
   assert(*r4 == 0x02);
   assert(*r2 == 0x0101);
   *r4 = 0x40;
   *r2 = 0;
   instructionsExecute(0x6444, ram, r);   /*overflow status*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0104);         /*overflow and negative*/

   *r4 = 0x0004;
   *r2 = 0x0001;
   instructionsExecute(0x5404, ram, r);  /*rla*/
   assert(*r4 == 0x0008);
   assert(*r2 == 0x0000);
   *r4 = 0x0000;
   *r2 = 0x0000;
   instructionsExecute(0x5404, ram, r);
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0002);          /*zero bit*/
   *r4 = 0xC000;
   *r2 = 0;
   instructionsExecute(0x5404, ram, r);   /*negative + carry*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0005);
   *r4 = 0x8001;
   *r2 = 1;
   instructionsExecute(0x5404, ram, r);   /*carry status + overflow*/
   assert(*r4 == 0x0002);
   assert(*r2 == 0x0101);
   *r4 = 0x4000;
   *r2 = 0;
   instructionsExecute(0x5404, ram, r);   /*overflow status*/
   assert(*r4 == 0x8000);
   assert(*r2 == 0x0104);         /*overflow and negative*/

   *r4 = 0x04;
   *r2 = 0x0001;
   instructionsExecute(0x5444, ram, r);  /*rla*/
   assert(*r4 == 0x08);
   assert(*r2 == 0x0000);
   *r4 = 0x00;
   *r2 = 0x0000;
   instructionsExecute(0x5444, ram, r);
   assert(*r4 == 0x00);
   assert(*r2 == 0x0002);          /*zero bit*/
   *r4 = 0xC0;
   *r2 = 1;
   instructionsExecute(0x5444, ram, r);   /*negative + carry*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0005);
   *r4 = 0x81;
   *r2 = 0;
   instructionsExecute(0x5444, ram, r);   /*carry status + overflow*/
   assert(*r4 == 0x02);
   assert(*r2 == 0x0101);
   *r4 = 0x40;
   *r2 = 0;
   instructionsExecute(0x5444, ram, r);   /*overflow status*/
   assert(*r4 == 0x80);
   assert(*r2 == 0x0104);         /*overflow and negative*/

   *r4 = 0x0004;
   instructionsExecute(0x9214, ram, r);  /*tst*/
   assert(*r4 == 0x0004);
   assert(*r2 == 1);
   *r4 = 0x0000;
   instructionsExecute(0x9214, ram, r);
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0xFFFF;
   instructionsExecute(0x9214, ram, r);
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0005);    /* negative + carry*/
   *r4 = 0x0001;
   *r2 = 0x0100;
   instructionsExecute(0x9214, ram, r);   /*no overflow + carry*/
   assert(*r4 == 0x0001);
   assert(*r2 == 0x0001);
   *r4 = 0x8001;
   *r2 = 0x0100;
   instructionsExecute(0x9214, ram, r);   /*no overflow + carry + negative*/
   assert(*r4 == 0x8001);
   assert(*r2 == 0x0005);

   *r4 = 0x04;
   instructionsExecute(0x9254, ram, r);  /*tst*/
   assert(*r4 == 0x04);
   assert(*r2 == 1);
   *r4 = 0x00;
   instructionsExecute(0x9254, ram, r);
   assert(*r4 == 0x00);
   assert(*r2 == 0x03);     /* carry + zero*/
   *r4 = 0xFF;
   instructionsExecute(0x9254, ram, r);
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0005);    /* negative + carry*/
   *r4 = 0x01;
   *r2 = 0x0100;
   instructionsExecute(0x9254, ram, r);   /*no overflow + carry*/
   assert(*r4 == 0x01);
   assert(*r2 == 0x0001);
   *r4 = 0x81;
   *r2 = 0x0100;
   instructionsExecute(0x9254, ram, r);   /*no overflow + carry + negative*/
   assert(*r4 == 0x81);
   assert(*r2 == 0x0005);

   *r4 = 0x0004;
   *r2 = 1;
   instructionsExecute(0x7304, ram, r);  /*carry*/
   assert(*r4 == 0x0004);
   assert(*r2 == 1);      /*carry*/
   *r4 = 0x0000;
   *r2 = 1;
   instructionsExecute(0x7304, ram, r);
   assert(*r4 == 0x0000);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0x0000;
   *r2 = 0;
   instructionsExecute(0x7304, ram, r);
   assert(*r4 == 0xFFFF);
   assert(*r2 == 0x0004);    /* negative*/
   *r4 = 0x8000;
   *r2 = 0;
   instructionsExecute(0x7304, ram, r);   /*overflow + carry*/
   assert(*r4 == 0x7FFF);
   assert(*r2 == 0x0101);

   *r4 = 0x06;
   *r2 = 1;
   instructionsExecute(0x7344, ram, r);
   assert(*r4 == 0x06);
   assert(*r2 == 1);
   *r4 = 0x00;
   *r2 = 1;
   instructionsExecute(0x7344, ram, r);
   assert(*r4 == 0x00);
   assert(*r2 == 0x0003);     /* carry + zero*/
   *r4 = 0x00;
   *r2 = 0;
   instructionsExecute(0x7344, ram, r);
   assert(*r4 == 0xFF);
   assert(*r2 == 0x0004);    /* negative*/
   *r4 = 0x80;
   *r2 = 0;
   instructionsExecute(0x7344, ram, r);  /*overflow + carry*/
   assert(*r4 == 0x7F);
   assert(*r2 == 0x0101);

   *r0 = 0x0000;
   *r1 = 0x0004;
   ram->memorySpace[0x0004] = 0x00FE;
   instructionsExecute(0x4130, ram, r);
   assert(*r1 == 0x0006);
   assert(*r0 == 0x0100);

   *r0 = 0x0008;
   *r1 = 0xFFFE;
   ram->memorySpace[0xFFFE] = 0x0004;
   instructionsExecute(0x4130, ram, r);
   assert(*r1 == 0x0000);
   assert(*r0 == 0x0006);
}

void testMemory(){
   memory *ram;
   int i;
   ram = memoryInit();
   for(i = 0; i < MEMORYSIZE; i++){
     assert(ram->memorySpace[i] == 0);
   }
}
