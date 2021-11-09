/* Program that counts consecutive 1s */
            .text                           
            .global _start                   
_start:          
            movia   r10, TEST NUM   # load the data word into r10
            ldw     r10, 0(r10)     
            movi    r15, 0          # r15 will hold the result

LOOP:       beq     r10, r0, END    # loop until the data contains no more 1's
            srli    r11, r10, 1     # perform SHIFT LEFT , followed by AND
            and     r10, r10, r11   
            addi    r15, r15, 1     # count length so far
            br      loop            

END:        br      end             

TEST NUM:   .word   0x103fe00f  

            .end                            
