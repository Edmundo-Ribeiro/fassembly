MOD_DIV: BEGIN
section text

  PUBLIC D
  PUBLIC R
  PUBLIC MOD_DIV
   COUNT: EXTERN
   BACKBUZZ: EXTERN
   BACKFIZZ: EXTERN
   CINCO: EXTERN
   TRES: EXTERN
   ZERO: EXTERN
   UM: EXTERN

  LOAD COUNT; ACC = COUNT
  LOOP:
    SUB D; ACC = COUNT - D
    JMPP LOOP; se acc ainda é positivo repete o loop

    COPY ZERO, R ; colocar 0 em R pra caso o numero não seja divisivel por D
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