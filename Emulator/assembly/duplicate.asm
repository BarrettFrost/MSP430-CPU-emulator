	.define LIST_IN		0x0200			;Memory allocation LIST_IN for original array
	.define LIST_OUT	0x0500			;Memory allocation LIST_OUT for processed unique-element array
			mov.w	#LIST_IN, R4
			mov.w	#6, 0(R4)
			mov.w	#1, 2(R4)
			mov.w	#1, 4(R4)
			mov.w	#3, 6(R4)
			mov.w	#4, 8(R4)
			mov.w	#4, 10(R4)
			mov.w	#4, 12(R4)

			mov.w	#LIST_IN, R4
			mov.w	#LIST_OUT, R5
			mov.w	@R4+, R6       ;R6 is the counter
			dec		R6
			mov.w	R5, R7         ;R7 points to the memory that records the new array's size
			clr.w	0(R7)          ;0(R7) records the size of new array
			incd	R5
LOOP:		cmp.w	@R4+, 0(R4)    ;Compar R4[i] and R4[i+1]
			jz		SKIP           ;If equal, keep looping
			mov.w	-2(R4), 0(R5)  ;If not equal, we have non-duplicate elements. Move a unique to new array
			incd	R5
			inc		0(R7)          ;Increment new array size
SKIP:		dec		R6
			jnz		LOOP
			mov.w	@R4, 0(R5)     ;When loop ends, always move the last old array element to new array
			inc		0(R7)
infinit:		call       refresh
                        BIS	   #00010h, SR
.org 00400h
refresh:
                        ret

.end