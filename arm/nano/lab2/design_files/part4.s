/* code for Part III (not shown) */

/* Display R5, R6 and R7 on the Terminal */
DISPLAY:    LDR     R0, =PRINT_R5   
            BL      PRINT_STR       // print the string

SHOW_R5:    MOV     R0, R5          // display content of R5 on the Terminal
            BL      PRINT_REG       // print the decimal digits
            ...
            code for printing "R6:" and decimal value of R6 (not shown)
            ...
            code for printing "R7:" and decimal value of R7 (not shown)
            ...
/* Subroutine to send a null-terminated string to the JTAG UART
 *     input: R0 contains the address of the string */
PRINT_STR:  LDR     R2, =0xFF201000 // JTAG UART base address
JTAG_LOOP:  LDRB    R1, [R0], #1    // get the next character
            ...
END_PRINT:  MOV     PC, LR  
        
/* Subroutine to "print" the decimal value of a register to the JTAG Terminal
 * 	input: R0 is the register to be converted to decimal and displayed */
PRINT_REG:  PUSH    {LR}            
            BL      DIVIDE          // ones digit will be in R0; tens digit
                                    // in R1
            LDR     R2, =0xFF201000 // JTAG UART base address
            ...
            POP     {PC}            
            ...

PRINT_R5:   .asciz  "R5: "      
            .skip   3           // pad with 3 bytes to maintain word alignment
PRINT_R6:   .asciz  "R6: "      
            .skip   2           // pad with 2 bytes to maintain word alignment
PRINT_R7:   .asciz  "R7: "      
            .skip   3           // pad with 3 bytes to maintain word alignment
