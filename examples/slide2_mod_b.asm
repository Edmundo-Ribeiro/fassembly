MOD_B: BEGIN
section text
A: EXTERN
L1: EXTERN
B:EXTERN
PUBLIC R
PUBLIC MOD_B
LOAD A
MUL B
STORE R
DIV DOIS
STORE R
JMP L1
section data
R: SPACE 
DOIS: CONST 2
END