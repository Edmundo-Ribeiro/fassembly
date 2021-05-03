section text

MOD_DIV: BEGIN
  PUBLIC D
  PUBLIC R
  PUBLIC MOD_DIV
  EXTERN COUNT
  EXTERN BACKBUZZ
  EXTERN BACKFIZZ
  EXTERN CINCO
  EXTERN TRES
  EXTERN ZERO
  EXTERN UM

  LOAD COUNT; ACC = COUNT
  LOOP:
    SUB D; ACC = COUNT - D
    JMPP LOOP; se acc ainda é positivo repete o loop

    COPY ZER0, R ; colocar 0 em R pra caso o numero não seja divisivel por D
    JMPN PULALINHA; se deu menor q zero é pq não é divisivel por D (ENTÃO PULA A PROXIMA LINHA)
    COPY UM, R ; colocar 1 em R pq se não deu o jump é pq acc = 0, ou seja, o numero é divisivel por D

    PULALINHA:
    LOAD TRES; acc = 3
    SUB D; acc = 3 - D
    JMPZ BACKFIZZ ; se der 0 volta pro fizz
    LOAD CINCO; acc = 5
    SUB D; acc = 5 - D
    JMPZ BACKBUZZ; se der 0 volta pro buzz

section data

D: SPACE
R: SPACE
END



section text
MAIN: BEGIN
  PUBLIC MAIN
  PUBLIC COUNT
  PUBLIC CINCO
  PUBLIC TRES
  PUBLIC BACKBUZZ
  PUBLIC BACKFIZZ

  EXTERN D
  EXTERN R

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
    COPY TRES, D; D = 5
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

BUZZ: CONST "buzz"
FIZZ: CONST "fizz"
UM: CONST 1
ZERO: CONST 0
CINCO: CONST 5
TRES: CONST 3
COUNT: SPACE

END