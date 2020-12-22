.include "msp430x2xx.inc"

mov #04297h, r4
dadd r4, r4
mov #00003h, r5
and #00001h, r5
mov #00003h, r6
bic #00001h, r6
mov #00003h, r7
bis #00001h, r7
mov #00003h, r8
xor #00001h, r8
BIS #00010h,SR

.end