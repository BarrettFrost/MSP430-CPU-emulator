CFLAGS = -Wall -Wfloat-equal -pedantic -ansi -O2 -lm
CC = gcc
OBJS = emulation

default:
	$(CC) $(CFLAGS)  instructions.c emulator.c memory.c format1.c format2.c format3.c decode.c flagHandler.c main.c display.c -o main

test:
	$(CC) $(CFLAGS) instructions.c emulator.c memory.c format1.c format2.c format3.c decode.c flagHandler.c test.c  -o testing

	#main: main.o emulator.o memory.o
	#$(CC) main.o emulator.o memory.o -o main

	#emulator.o: emulator.c
	#$(CC) $(CFLAGS) -c emulator.c

	#memory.o: memory.c
	#$(CC) $(CFLAGS) memory.c

	#main.o: main.c
	#$(CC) $(CFLAGS) main.c
