

start:
mov #0, r4
mov #42, r5
call r5
call 0(R5)
call &add
call add
call #42
mov #01000h, r5
mov #42, 0(R5)
call @r5
call @r5+
BIS #00010h, r2

add:
INC r4
ret
.end