Mod_a:
SECTION DATA
r: SPACE
n: SPACE
um: CONST 1
zero: CONST 0
TRES: CONST 3

SECTION TEXT
mod_b: extern
mod_c: extern
public n
public R
public um
public PULALINHA
public loop
input N
load N
loop: sub TRES
JMPP loop
COPY zero, R
jmp mod_b
jmp mod_c
JMPN PULALINHA
COPY um, R
PULALINHA:
OUTPUT R
jmp mod_b
STOP


