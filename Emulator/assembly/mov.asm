.include "msp430x2xx.inc"

.define A 0x0007
.define B 0x0005

mov #6h, R4      ;r4 = 6
MOV R4, R5       ;r5 = 6
MOV #3h, 1(R5)   ; @0x0007 = 3
mov 1(R5), R6    ;r6 = 3
mov A, R7        ;r7 = 3
mov #7h, B       ;@0x0005 = 7
mov &B, R8       ;r8 = 7
mov @R8, R9      ;r9  = 3
mov @R8+, R10    ;r10 = 3, r8 = 9
mov #2000, R11   ; r11 = 2000
mov.b #127, R11  ; r11 = 127
mov.b @R8+, R10  ;r10 = 0, r8 = 10
BIS #00010h,SR
          .end
