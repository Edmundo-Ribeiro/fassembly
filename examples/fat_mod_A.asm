MOD_A: BEGIN  ;010101010001000
SECTION DATA
	N: SPACE
	ONE: CONST 1
SECTION TEXT
        MOD_B: EXTERN
	PUBLIC FAT
	PUBLIC N
	INPUT N
	LOAD N
	FAT: SUB ONE
	JMPZ FIM
	JMP MOD_B
	FIM: OUTPUT N
	STOP

END