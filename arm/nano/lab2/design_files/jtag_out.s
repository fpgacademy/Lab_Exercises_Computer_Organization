/********************************************************************************
 * Subroutine to send a character to the JTAG UART
 * R0 = character to send
 ********************************************************************************/
PUT_JTAG:   LDR     R1, =0xFF201000 // JTAG UART base address
            STR     R0, [R1]        // send the character
END_PUT:    BX      LR              
