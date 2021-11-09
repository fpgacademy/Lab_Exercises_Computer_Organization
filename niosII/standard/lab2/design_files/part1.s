/* Program that counts consecutive 1's */

            .text                               
            .global _start                      
_start:                                 
            ldw     r9, TEST_NUM(r0)    # load the data into r9

            mov     r10, r0             # r10 will hold the result
LOOP:       beq     r9, r0, END         # loop until r9 contains no more 1s
            srli    r11, r9, 0x01       # count the 1s by shifting the number
            and     r9, r9, r11         # and ANDing it with the shifted
                                        # result
            addi    r10, r10, 0x01      # increment the counter
            br      LOOP                

END:        br      END                 # wait here

TEST_NUM:   .word   0x103fe00f          # the number to be tested        

            .end                                
