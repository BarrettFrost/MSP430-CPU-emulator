
start:

mov 00200h, r1
mov.b #0, r4
call &for
mov.b #0, r5
mov.b #1, r6
mov.b #0, r7
call &while
BIS #00010h,SR

for:
CMP.b #10, R4
jz for_done
add.b #1, r4
jmp for

for_done:
ret

while:
bit.b #1, r5
jnz while_done
rla.b r6
call &if
add.b #1, r7
jmp while

if:
cmp.b #0, r6
jz else  
ret

else:
mov.b #1, r5
ret

while_done:
ret 
.end