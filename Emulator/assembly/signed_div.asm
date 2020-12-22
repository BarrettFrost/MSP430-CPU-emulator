.define A  4
.define B  2
.define C -15
.define D 3

; signed division of integers

main:
mov #0, r8
mov #A, r4
mov #B, r5
mov #8000h, r6   ; most significant bit is 1
and r4, r6
mov #8000h, r7
and r5, r7
mov #0, r10
call &flip      
INV r5
add #1, r5
call &div
mov r4, r11   
mov r8, r10    ; result in r10

mov #0, r8
mov #C, r4
mov #D, r5
mov #8000h, r6
and r4, r6
mov #8000h, r7
and r5, r7
call &flip
INV r5
add #1, r5
call &div
mov r8, r11    ; result in r11
jmp finish

flip:
cmp #08000h, r6   ; if negative turn number positive
jz flipNum
cmp #08000h, r7   ; if negative turn number positive
jz flipDenom
ret

flipNum:

INV r4           ; Invert then add 1
add #1, r4
cmp #08000h, r7
jz flipDenom
ret

flipDenom:
INV r5            ; Invert then add 1
add #1, r5
ret

div:
cmp r6, r7
jz posResult
jmp negResult


negResult:

cmp #0, r4
jl done 
cmp #0, r4
jz done
add r5, r4     ; add numurator and denom
sub #1, r8     ; counts number denominators in numurator negative result
jmp negResult

posResult:

cmp #0, r4
jl done 
cmp #0, r4
jz done
add r5, r4      ; add numurator and denom
add #1, r8      ; counts number denominators in numurator
jmp posResult

done:
ret


finish:
BIS #00010h,SR

.end