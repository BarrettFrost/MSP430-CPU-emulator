.include "msp430x2xx.inc"

start:
mov #00300h, r1
push #8
pop  r4
call &called
setc
mov #2, r5
rlc r5
setn
setz
tst r7
mov #4, r6
sbc r6
BIS #00010h,SR
called:
ret 

end.