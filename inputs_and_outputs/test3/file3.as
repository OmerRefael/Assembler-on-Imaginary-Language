LABEL: jmp *r4

; label that already exist
LABEL: cmp #-3, *r0

; name of label contains illegal chars
NOT_VALID_LABEL: prn #5

; incorrect amount of operands
clr r1,LABEL

; incorrect amount of operands
stop hhhh

; illegal adressing method for source operand
lea #54, *r7

; illegal adressing method for dest operand
add   r1, #800

; illegal adressing method for dest operand
jsr #-100

; invalid operand
inc ^@^@

; invalid value for the register in the indirect adressing method
mov #23, *r9

; operand of immidate adressing method is not integer number
prn #NOTNUMBER

; label not start with letter
5FUNC:  inc r6 
