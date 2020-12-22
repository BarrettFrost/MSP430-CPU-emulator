# MSP430-CPU-emulator
This emulator was created following the specifications provided by Texas Instruments in 
https://www.ti.com/lit/ug/slau144j/slau144j.pdf?ts=1608559182115&ref_url=https%253A%252F%252Fwww.google.de%252F
This is an MPS430 CPU emulator
instructions are loaded at memory address 0
This emulator was created in Linux (Ubuntu windows Linux subsystem) 
using a GCC compiler
The programs were created with the naken_asm assembler avialable at
https://github.com/mikeakohn/naken_asm
The assembly is avialable in the assembly folder

To compile the emulator

make

To run the emulator execute ./main programs/(program you want to run)

./main programs/mov.bin

To run the emulator with breakpoint

./main programs/mov.bin breakpoint 10

To run the emulator whilst displaying an array at memory address 0x0400.
Everytime program counter holds memory address 0x0500 an array or 
memory contents is printed.


./main programs/pulsar.bin display 15
./main programs/conway.bin display 3


To compile the testing file

make test

To run the testing file

./testing
