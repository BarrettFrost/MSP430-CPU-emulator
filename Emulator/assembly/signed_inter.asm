.define X 00300h    ; address for x values
.define Y 00310h    ; address for y values
.define coord 4     ; amount of coordinates
.define register 00700h
.define index 001000h   ;store index
.define signN 00800h    ; sign of numerator
.define signD 00801h    ; sign of denominator
; signed newton interpolation
; 2s complement with 8 bit fixed point representation
.org 0xC000h
init:

mov #X, r4
mov #00300h, 0(r4)  ;3
mov #00400h, 1(r4)  ;4
mov #00500h, 2(r4)  ;5
mov #00700h, 3(r4)  ;7
mov #Y, r4
mov #0FE00h, 0(r4)  ;-2
mov #00A00h, 1(r4)  ;10
mov #01000h, 2(r4)  ;16
mov #02200h, 3(r4)  ;34
mov #1, r14
mov #X, r4
mov #Y, r5
mov #1, r11
mov r5, r15
add #coord, r15
mov #1, &index
jmp main

main:

cmp #coord, r11  ; number of loops 
jz finish
add #1, r11
call &loop
mov #X, r4       ; reset the address to beginning of X coordinates
add #1, r5       ; reset the address to beginning of X coordinates
add #1, &index
mov &index, r14
jmp main


loop:
cmp #coord, r14
jz done
mov 1(r5), r6    ; load y coordinate 
mov @r5, r7      ; load other y coordinate
mov &index, r13
add r4, r13
mov @r13, r8     ; load x coordinate
mov @r4, r9      ; load other x coordinate
sub r7, r6       ;y - y
sub r9, r8       ;x - x

mov #8000h, &signN
and r6, &signN
mov #8000h, &signD
and r8, &signD
call &flip
INV r8
add #1, r8

call &div
call &fraction
add #1, r14        ; add one to the index
add #1, r4         ; increment address
add #1, r5         ; increment address
mov r12, 0(r15)     ; store result 
add #1, r15         ; increment address
jmp loop

flip:
cmp #08000h, &signN   ; if negative turn number positive
jz flipNum
cmp #08000h, &signD   ; if negative turn number positive
jz flipDenom
ret

flipNum:

INV r6                 ; Invert then add 1
add #1, r6
cmp #08000h, &signD
jz flipDenom
ret

flipDenom:
INV r8              ; Invert then add 1
add #1, r8
ret

div:
cmp &signD, &signN
jz posResult
jmp negResult


negResult:

cmp #0, r6
jl done 
cmp #0, r6
jz done
add r8, r6             ; add numurator and denom
sub #1, r10            ; counts number denominators in numurator negative result
jmp negResult

posResult:

cmp #0, r6
jl done 
cmp #0, r6
jz done
add r8, r6              ; add numurator and denom
add #1, r10             ; counts number denominators in numurator negative result
jmp posResult

fraction:

mov r10, r12   ; divide remainder by the denominator 
mov #0, r10
call &div
mov #0, r13
call &shift    ; shift the right by eight the create the fraction
add r10, r12   ; add fraction to the result
ret

shift:
cmp #8, r13
jz done
add #1, r13
clrc
rlc r12
jmp shift

finish:
mov #coord, r15      ;move results into registers
add #Y, r15
mov @r15, r4
add #3, r15
mov @r15, r5
add #2, r15
mov @r15, r6
BIS #00010h,SR


done:
ret


