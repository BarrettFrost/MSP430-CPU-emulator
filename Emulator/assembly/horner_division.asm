; horner division integer by float 

.define X 001B9h
.define M 0061Eh   ; reciprical
.define diff 00300h
.define print 00700h
; 441 / 41.8375 = 10.5407  6 bits for fraction
start:
mov #X, r4
mov #M, r5
mov #1, r6  ;   for the AND compare    start from left most bit
mov #0, r7
mov #0, r11
mov #diff, r12
call &findBits
mov #1, r7
mov #diff, r12
add #1, r12      ;address of the first difference
mov r4, r8
jmp mul

findBits:
cmp #12, r7   ;number of bits in multiplier (M) need to change for different multipler
jz done
add #1, r7
add #1, r13
mov r5, r8
and r6, r8
cmp r6, r8
jeq countBit    ; count the bit
rla r6           ; move the 1 in startBit the the left;
jmp findBits

countBit:
add #1, r9     ;number of 1s in multipler
mov r13, @r12  ; stores difference between nearest 1 in multiplier
add #1, r12    ; changes the address 
mov #0, r13    ; zeros for next difference
rla r6         ; move the 1 in startBit the the right;
jmp findBits

mul:
cmp r9, r7     ; number of 1s in multiplier
jz last
mov @r12, r14  ;move difference between 1s
call rotate
add #X, r8    ; add X the rotated result
add #1, r12    ; move address
add #1, r7
mov #0, r11
jmp mul

rotate:

cmp r14, r11
jz done
add #1, r11
rra r8            ;shift right the X by the difference
jmp rotate

last:
mov #08000h, r6   ; depends on format used
mov #1, r10
call findWeight   ; finds weight of left most bit
call rotate       ; right shift the result
jmp finish

findWeight:
mov #M, r15
and r6, r15
cmp r6, r15
jz donefind
clrc
rrc r6
add #1, r10    ; r10 weight
jmp findWeight

donefind:
mov r10, r14
ret

done:
ret

finish:
BIS #00010h,SR     ; r8 has result

.end
