.include "msp430x2xx.inc"

mov #0FFFFh, r4
setc
adc r4
br #036h

continue:
mov #10, r6
add #0FFFFh, r6
clrc 
mov #1, r6
sub #1, r6
clrz
mov #0, r6
sub #1, r6
clrn
setc 
dadc r7
dec r8
decd r9
DINT
EINT
inc r10
incd r11
inv r12
nop
BIS #00010h,SR

branch:
mov #4, r5
clr r5
br #8





