Mod_a:
SECTION DATA
r: SPACE ; 25
n: SPACE ; 26
um: CONST 1 ;27
zero: CONST 0 ;28
TRES: CONST 3 ;29


SECTION TEXT
mod_b: extern
mod_c: extern
public n
public R
public um
public PULALINHA
PUBLIC RAND
public loop
input N; 0 1
load N; 2 3
loop: sub TRES; 4 5
PULALINHA:RAND: JMPP loop; 6 7
COPY zero, R; 8 9 10
jmp mod_b; 11 12
jmp mod_c; 13 14
JMPN RAND; 15 16
COPY um, R; 17 18 19
OUTPUT R; 20 21
jmp mod_b; 22 23
STOP; 24


