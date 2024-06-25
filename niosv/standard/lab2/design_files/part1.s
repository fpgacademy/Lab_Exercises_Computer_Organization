# Program that counts consecutive 1's

.global _start
_start:     la      t0, test_num    # t0 = address of data word
            lw      t0, (t0)        # t0 = data word

            li      t1, 0           # t1 will hold the result
loop:       beqz    t0, end         # loop until t0 has no more 1s
            srli    t2, t0, 1       # shift and then ...
            and     t0, t0, t2      # and to count the 1s
            addi    t1, t1, 1       # increment counter
            j       loop

end:        j       end             # wait here

test_num:   .word   0x103fe00f      # sample data
