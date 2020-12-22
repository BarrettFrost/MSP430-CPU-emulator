
start:

mov #00200h, r1
mov #16, r4
push  r4
mov #1, r4
push r4
reti
add r4, r4 ; skips this intrustion
mov r2, r5
BIS #00010h,SR
.end