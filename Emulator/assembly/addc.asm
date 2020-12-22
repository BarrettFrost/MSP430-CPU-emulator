; 32 bit addition 100,000 + 150,000

mov #00002h, r4  ; 150,000
mov #049F0h, r5
mov #00001h, r7
mov #086A0h, r8
add r5, r8
addc r4, r7
BIS #00010h,SR
.end