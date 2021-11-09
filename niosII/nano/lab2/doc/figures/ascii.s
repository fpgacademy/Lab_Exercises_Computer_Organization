/******************************************************************************
* Subroutine to send a character to the JTAG UART
* r3 = character to send
******************************************************************************/
PUT_JTAG:   movia   r10, 0xFF201000  # JTAG UART base address
            stw     r3, 0(r10)       # send the character
END_PUT:    ret
