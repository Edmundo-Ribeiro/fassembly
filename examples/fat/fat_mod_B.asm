MOD_B: BEGIN
FAT: EXTERN
	N: EXTERN
	PUBLIC MOD_B
SECTION DATA
	AUX: SPACE
SECTION TEXT    ;01000001000
	STORE AUX
	MUL N
	STORE N
	LOAD AUX
	JMP FAT

END