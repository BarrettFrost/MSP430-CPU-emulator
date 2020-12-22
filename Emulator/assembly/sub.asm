.include "msp430x2xx.inc"

mov #10, r4
sub #6, r4
mov.b #012h, r5
mov.b #023h, r6
mov.b #056h, r7
mov.b #078h, r8   0x5678 - 0x1223 = 17493
sub.b r6, r8
subc.b r5, r7
mov.b #078h, r9
mov.b #056h, r10
mov.b #12h, r11
mov.b #23h, r12
sub.b r10, r12
subc.b r9, r11  0x1223 - 0x7856 = -26163
BIS #00010h,SR
.end