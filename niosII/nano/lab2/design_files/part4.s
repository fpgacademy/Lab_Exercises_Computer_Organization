...
code for Part III (not shown)
...
/* Display r15, r16 and r17 on the Terminal */
DISPLAY:    movia   r3, PRINT_R15   
            call    PRINT_STR       # print the string

SHOW_R5:    mov     r10, r15        # display content of r15 on the Terminal
            call    PRINT_REG       # print the decimal digits
            ...
            code for printing "r16:" and decimal value of r16 (not shown)
            ...
            code for printing "r17:" and decimal value of r17 (not shown)
            ...

/* Subroutine to send a null-terminated string to the JTAG UART
 * 	input: r3 contains the address of the string */
PRINT_STR:  movia   r6, 0xFF201000  # JTAG UART base address
JTAG_LOOP:  ldb     r2, 0(r3)       # get the next character
            ...
END_PRINT:  ret    

/* Subroutine to "print" the decimal value of a register to the JTAG Terminal
 * 	input: r3 is the register to be converted to decimal and displayed */
PRINT_REG:  mov     r19, ra         # save return address
            call    DIVIDE          # ones digit will be in r2; tens digit in
                                    # r3
            movia   r6, 0xFF201000  # JTAG UART base address
            ...
            mov     ra, r19         # restore return address

            ...

PRINT_R15:  .asciz  "r15: "     
            .skip   2           # pad with 2 bytes to maintain word alignment
PRINT_R16:  .asciz  "r16: "     
            .skip   2           # pad with 2 bytes to maintain word alignment
PRINT_R17:  .asciz  "r17: "     
            .skip   2           # pad with 2 bytes to maintain word alignment
