

.define display 00500h
.define copy 00600h
.define registers 00700h

; conways game of life 15 x 15 pulsar

init:

mov #0, r4
mov #1, r5
mov #4, r8
mov #display, r9
call &initArray
mov #copy, r9    ; copy of the array
call &initArray
call &refresh
mov #display, r9
mov #0, r10    ; row counter
mov #0, r6     ; column counter
mov #0, r7     ; game counter
jmp loop


initArray:

mov #1, 4(r9)
mov #1, 10(r9)
mov #1, 19(r9)
mov #1, 25(r9)
mov #1, 34(r9)
mov #1, 35(r9)
mov #1, 39(r9)
mov #1, 40(r9)
mov #1, 60(r9)
mov #1, 61(r9)
mov #1, 62(r9)
mov #1, 65(r9)
mov #1, 66(r9)
mov #1, 68(r9)
mov #1, 69(r9)
mov #1, 72(r9)
mov #1, 73(r9)
mov #1, 74(r9)
mov #1, 77(r9)
mov #1, 79(r9)
mov #1, 81(r9)
mov #1, 83(r9)
mov #1, 85(r9)
mov #1, 87(r9)
mov #1, 94(r9)
mov #1, 95(r9)
mov #1, 99(r9)
mov #1, 100(r9)
mov #1, 124(r9)
mov #1, 125(r9)
mov #1, 129(r9)
mov #1, 130(r9)
mov #1, 137(r9)
mov #1, 139(r9)
mov #1, 141(r9)
mov #1, 143(r9)
mov #1, 145(r9)
mov #1, 147(r9)
mov #1, 150(r9)
mov #1, 151(r9)
mov #1, 152(r9)
mov #1, 155(r9)
mov #1, 156(r9)
mov #1, 158(r9)
mov #1, 159(r9)
mov #1, 162(r9)
mov #1, 163(r9)
mov #1, 164(r9)
mov #1, 184(r9)
mov #1, 185(r9)
mov #1, 189(r9)
mov #1, 190(r9)
mov #1, 199(r9)
mov #1, 205(r9)
mov #1, 214(r9)
mov #1, 220(r9)
ret

loop:

cmp #5, r7    ; 5 generations
jz done
jmp game

game:

CMP #15, r10  ; 15 rows
jz repeat
cmp #15, r6   ; 15 column
jz nextRow
mov r9, r11   ; move the cell address to r11
call &checkSurround  
add #1, r9    ; increment cell address
add #1, r6    ; increment column
mov #0, r12
jmp game

nextRow:
mov #0, r6        ; reset the column
add #1, r10       ; increment row
br #0011Ch        ; needs to change

repeat:
mov #display, r9  ; move cell pointer to start of array
call &temp
call &refresh     ; print the generation
mov #display, r9  ; move cell pointer to start of array
mov #0, r10       ; reset row counter
mov #0, r6        ; reset column counter
add #1, r7        ; increment generation
jmp loop

checkSurround:

call &checkLeft   
mov r9, r11        ; reset to center cell
call &checkRight
mov r9, r11
call &checkTop
call &checkBottom
CMP @r9, r5
jz LiveToDead
JMP DeadToLive

checkLeft:

CMP #0, r6                ;column is zero no left cells
jz finish
sub #16, r11              ;top left
call &TopDiagonalCheck
add #15, r11              ; left
call &check
add #15, r11               ; bottom left
call &BottomDiagonalCheck
ret

checkRight:

CMP #14, r6               ;column is 14 no right cells
jz finish
sub #14, r11              ;top right
call &TopDiagonalCheck
add #15, r11              ; right
call &check
add #15, r11              ; bottom right
call &BottomDiagonalCheck
ret

checkTop:

CMP #0, r10         ;row is 0 no top cell
jz finish
sub #15, r11        ; top
call &check
add #15, r11
ret

checkBottom:

CMP #14, r10     ;row is 14 no top cell
jz finish
add #15, r11     ;bottom
call &check
sub #15,r11
ret

LiveToDead:
CMP #2, r12     ; if there is less than 2 surronding cells
jl kill
CMP #4, r12     ; if there is 4 or more surronding cells
jge kill
ret

kill:
mov #0, 256(r9)  ; kill a cell in the copy
ret

DeadToLive: 
CMP #3, r12    ; if there is 3 surronding cells
jz Birth
ret

Birth:
mov #1, 256(r9)  ; birth a cell in the copy
ret

temp:
cmp #005E1h, r9    ;move the copy board to the display area
jz finish
mov 256(r9), 0(r9)
add #1, r9
jmp temp

isAlive:         ; count the number of alive cells in the surrounding
add #1, r12
ret

check:

CMP @r11, r5     ;compare contents of cell with 1
jz isAlive
ret

TopDiagonalCheck:

CMP #1, r10         ; if row is 0 no top diagonal cell
jge check
ret

BottomDiagonalCheck:

CMP #14, r10        ; if row is 14 no bottom diagonal cell
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
