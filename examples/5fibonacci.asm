
section data
one:        const   1
minus_one:
                    const   -1
count:  const 5    ; numbers to
            ; print
old:    space
now:    space
temp:   space
section text
start:

; Starting the code now

        LoAd    one   ; load first
        STORE   old   ; store as f-1
        store   now   ; store as: f
        output  old   ; show first

        output  now   ; show second
        load count;
            add minus_one
       ADD minus_one
        stOre cOUNT
loop:   load    now   
    ; get now for accumulator
add old ; make sum
        copy    now,old ; move now to old
store   now ; save ACC on now
                        output now ; show now
        load    count
        sub             one
        store   count
        jmpp loop
        jmp end
end:stop