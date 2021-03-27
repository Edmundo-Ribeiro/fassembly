 section data
one:        CONST 1
two:        CONST -2


            section text
start:      add     one                 ; Simple add something
            jmp     end
; Let's finish this empty file
end:        stop
     