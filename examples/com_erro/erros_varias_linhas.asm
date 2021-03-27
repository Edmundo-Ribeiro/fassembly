            section data
one:        CONS  1
two:        CONST -2
result:     SPACE
start:      SPACE

; Simple error testing
            section text
start:      ad      one,top                 ; Simple add something
            div     two,top
            STORE   result
            Jmp     end
            jmpp    start
end:        stop