MOD_A: BEGIN
section text
R: EXTERN
MOD_B: EXTERN
PUBLIC A
PUBLIC B
PUBLIC L1
INPUT A
INPUT B
JMP MOD_B
L1: OUTPUT R
STOP
section data
A: SPACE 
B: SPACE 
END