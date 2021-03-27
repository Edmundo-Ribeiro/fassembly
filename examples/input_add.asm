
            section data
first_op:   SPACE
second_op:  SPACE
result:     SPACE

            section text
start:                                      ; This is a beautifully formatted code
            INPUT   first_op                ; Read first operand
            INPUT   second_op               ; Read seconde operand
            LOAD    first_op                ; Load first operand into ACC
            ADD     second_op               ; ACC <- ACC + second operand
            STORE   result                  ; Store result for output
            OUTPUT  result                  ; Show result
            STOP