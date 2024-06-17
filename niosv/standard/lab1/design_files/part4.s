# Program that reverses a text string

            .text
            .global _start
_start:     la      s0, string   # s0 pointer to first character
            mv      s1, s0
zloop:      lb      t0, (s1)     # read the character
            beqz    t0, cont     # end of string?
            addi    s1, s1, 1    # point to next character
            j       zloop

cont:       addi    s1, s1, -1   # s1 = pointer to last character
reverse:    bge     s0, s1, stop
            lb      t0, (s0)     # swap characters
            lb      t1, (s1)
            sb      t0, (s1)
            sb      t1, (s0)

            addi    s0, s0, 1
            addi    s1, s1, -1
            j       reverse

stop:       j       stop         # wait here

            .data
string:     .asciz  "gnirts a si siht"
