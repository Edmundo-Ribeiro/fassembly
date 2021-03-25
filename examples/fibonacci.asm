        COPY ZERO,OLDER    ; 0
        COPY ONE,OLD       ; 3
        INPUT LIMIT        ; 6
        OUTPUT OLD         ; 8
FRONT:  LOAD OLDER         ; 10
        ADD OLD            ; 12
        STORE NEW          ; 14
        SUB LIMIT          ; 16
        JMPP FINAL         ; 18
        OUTPUT NEW         ; 20
        COPY OLD,OLDER     ; 22
        COPY NEW,OLD       ; 25
        JMP FRONT          ; 28
FINAL:  OUTPUT LIMIT       ; 30
        STOP               ; 32
ZERO:   CONST 0            ; 33
ONE:    CONST 1            ; 34
OLDER:  SPACE              ; 35
OLD:    SPACE              ; 36
NEW:    SPACE              ; 37
LIMIT:  SPACE              ; 38