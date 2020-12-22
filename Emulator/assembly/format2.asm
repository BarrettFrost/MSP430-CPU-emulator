.define address 00300h

start:
mov.b #150, r4
rrc.b r4
mov.b #200, r5
setc
rrc.b r5
mov #-8, r6
rra r6
mov #00FFFh, r7
swpb r7
mov #000FFh, r8
sxt r8
mov #address, r1
PUSH #0FFFFh
PUSH.b #0FFh
mov #address, r11
sub #4, r11
mov @r11+, r12
mov @r11, r13 
call &register
mov #1000, r10
BIS #00010h,SR
register:
mov #1000, r9
ret

.end