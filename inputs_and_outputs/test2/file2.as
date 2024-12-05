;undefined command
what r4, label

;missing comma
label: mov r3 *r4

;multiple comma
.data -4 ,  ,6 

; missing paramter for data guidance
.data 

; invalid type for data guidance member
.data NOT_NUMERR

; invalid type for data guidance member
.data 15.8

; missing paramter for strind guidance
.string 

;empty string
.string ""

; missing open quote for the string paramter
.string word"

; missing close quote for the string paramter
.string "word
