; horner multiplication integers

.define X 41
.define M 441
.define diff 00300h
.define print 00700h
.define startBit 00100h     ; start from right most bit

; 0.12345 * 0.14325 = 0.0176842125

start:
mov #X, r4
mov #M, r5
mov #startBit, r6     ;for the AND compare
mov #0, r7
mov #0, r11
mov #diff, r12
mov #0, r9
call &findBits
mov #1, r7
mov #diff, r12
add #1, r12         ;address of the first difference
mov r4, r8
jmp mul

findBits:
cmp #9, r7  ;number of bits in multiplier (M) need to change for different multipler
jz done
add #1, r7
add #1, r13
mov r5, r8
and r6, r8    
cmp r6, r8    
jeq countBit  ; count the bit
clrc
rrc r6      ; move the 1 in startBit the the right;
jmp findBits

countBit:
add #1, r9      ;number of 1s in multipler
mov r13, @r12   ; stores difference between nearest 1 in multiplier
add #1, r12     ; changes the address 
mov #0, r13     ; zeros for next difference
rrc r6          ; move the 1 in startBit the the right;
jmp findBits

mul:
cmp r9, r7    ; number of 1s in multiplier
jz last      
mov @r12, r14  ;move difference between 1s
call rotate    
add #X, r8    ; add X the rotated result
add #1, r12   ; move address
add #1, r7    
mov #0, r11   
jmp mul

rotate:

cmp r14, r11  
jz done
add #1, r11
rlc r8          ;shift right the X by the difference
jmp rotate

last:
jmp finish       ;r8 has result

done:
ret

finish:
BIS #00010h,SR

.end
