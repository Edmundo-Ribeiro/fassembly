MOD_A: BEGIN

SECTION DATA
VAL: CONST 5; 11

SECTION text
Y: EXTERN
MOD_B: EXTERN
PUBLIC VAL
PUBLIC L1
INPUT Y; 0 1
LOAD VAL; 2 3
ADD Y; 4 5
STORE Y; 6 7
JMPP MOD_B; 8 9
L1: STOP ; 10

END