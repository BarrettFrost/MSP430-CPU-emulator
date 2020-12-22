; gets the 12th fibonacci numbers

init:

mov #10, r4     ; which fibonacci number 10 + 2 = 12
mov #00050h, r5  ; first number
mov #00051h, r6  ; second number
mov #1, 0(r6)     ; second fibonacci number    
jmp loop

loop:
mov @r6, r7       
add.b @r5+, 0(r6)   ; add the 2 numbers
mov 0(r6), 1(r6)    ; move number into the sequence
mov r7, 0(r6);      
add #1, r6          ; increment the address
sub #1, r4
jz print
jmp loop
print:
mov 0(r6), r8    ; the 12th fibonacci number
BIS #00010h, SR

.end