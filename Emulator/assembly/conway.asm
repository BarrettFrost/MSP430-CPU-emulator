.include "msp430x2xx.inc"

.define display 00500h
.define registers 00700h

init:

mov #0, r4
mov #1, r5
mov #4, r8
mov #display, r9
mov #0, r10
call &initArray
call &refresh
mov #display, r9
mov #0, r10    ; row counter
mov #0, r6     ; column counter
mov #0, r7     ; game counter
jmp loop


initArray:

mov #0, 0(r9)
mov #1, 1(r9)
mov #0, 2(r9)
mov #0, 256(r9)
mov #1, 257(r9)
mov #0, 258(r9)
add #3, r9
add #1, r10
CMP #3, r10
jz finish
jmp initArray

loop:

cmp #2, r7
jz done
jmp game

game:

CMP #3, r10
jz repeat
cmp #3, r6
jz nextRow
mov r9, r11
call &checkSurround
add #1, r9
add #1, r6
mov #0, r12
jmp game

nextRow:
mov #0, r6
add #1, r10
br #00058h

repeat:
mov #display, r9
call &temp
call &refresh
mov #display, r9
mov #0, r10     ; row counter
mov #0, r6      ; column counter
add #1, r7
jmp loop

checkSurround:

call &checkLeft
mov r9, r11
call &checkRight
mov r9, r11
call &checkTop
call &checkBottom
CMP @r9, r5
jz LiveToDead
JMP DeadToLive

checkLeft:

CMP #0, r6
jz finish
sub #4, r11  ;top left
call &TopDiagonalCheck
add #3, r11
call &check
add #3, r11  ; bottom left
call &BottomDiagonalCheck
ret

checkRight:

CMP #2, r6
jz finish
sub #2, r11  ;top right
call &TopDiagonalCheck
add #3, r11
call &check
add #3, r11  ; bottom right
call &BottomDiagonalCheck
ret

checkTop:

CMP #0, r10
jz finish
sub #3, r11
call &check
add #3, r11
ret

checkBottom:

CMP #2, r10
jz finish
add #3, r11
call &check
sub #3,r11
ret

LiveToDead:
CMP #2, r12
jl kill
CMP #4, r12
jge kill
ret

kill:
mov #0, 256(r9)
ret

DeadToLive:
CMP #3, r12
jz Birth
ret

Birth:
mov #1, 256(r9)
ret

temp:
cmp #00509h, r9
jz finish
mov 256(r9), 0(r9)
add #1, r9
jmp temp

isAlive:
add #1, r12
ret

check:

CMP @r11, r5
jz isAlive
ret

TopDiagonalCheck:

CMP #1, r10
jge check
ret

BottomDiagonalCheck:

CMP #2, r10
jl check
ret

finish:
ret

done:
BIS #00010h,SR

.org 00400h
refresh:
ret
.end
