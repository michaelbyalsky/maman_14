MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
bne LOOP
sub @r1, @r4
    bne END
L1: inc K
bne LOOP
sub @r5, @r8
    bne END
END: stop
sub @r5, @r8
    bne END
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22