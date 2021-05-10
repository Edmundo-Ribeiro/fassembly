MAIN: BEGIN
section text
  PUBLIC MAIN
  PUBLIC COUNT
  PUBLIC CINCO
  PUBLIC TRES
  PUBLIC BACKBUZZ
  PUBLIC BACKFIZZ
  PUBLIC ZERO

  D: EXTERN
  R: EXTERN
  MOD_DIV: EXTERN

  COPY ZERO, COUNT; COUNT = 0
  INPUT N

  WHILE: 
    OUTPUT COUNT 
    LOAD COUNT
    ADD UM 
    STORE COUNT ;COUNT = COUNT + 1

    ;VER SE É DIVISIVEL POR 3

    COPY TRES, D; D = 3
    COPY ZERO, R; R =0
    JMP MOD_DIV
    BACKFIZZ: LOAD R
    JMPZ PULAF
    OUTPUT FIZZ

    PULAF: 
    ;VER SE É DIVISIVEL POR 5
    COPY CINCO, D; D = 5
    COPY ZERO, R; R =0
    JMP MOD_DIV
    BACKBUZZ: LOAD R
    JMPZ PULAB
    OUTPUT BUZZ

    PULAB: 
    LOAD N; acc = N
    SUB COUNT ;acc = N - COUNT
    JMPZ WHILE; se ainda não chegou em N repte tudo
    STOP

section data
N:SPACE
BUZZ: CONST 66
FIZZ: CONST 70
UM: CONST 1
ZERO: CONST 0
CINCO: CONST 5
TRES: CONST 3
COUNT: SPACE

END