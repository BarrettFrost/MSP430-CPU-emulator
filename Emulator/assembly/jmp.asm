.include "msp430x2xx.inc"

.entry_point start

start:

mov #0, r4
mov #1, r5
cmp r5, r4
jne random 


overflow:
add #1, r4
cmp r7, r6
jl last

Nocarry:
add #1, r4
jnc carry 

random:
add #1, r4;
cmp r5, r4
jeq Nocarry

negative:
add #1, r4
mov #-1, r6
mov #0, r7
cmp r7, r6
jn gl

last:
add #1, r4
jmp stop

gl:
add #1, r4
cmp r7,r4
jge overflow

carry:
add #1, r4
mov #5, r5
add.b #255, r5
jc negative


stop:
BIS #00010h,SR


.end