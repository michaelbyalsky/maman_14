MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
.extern L3
bne LOOP
sub @r1, @r4
bne END
L1: inc K
bne LOOP
         sub @r4, @r7
bne END
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
