/* Subroutine to convert the digits from 0 to 9 to be shown on a HEX display.
 *	Parameters: r4 = the decimal value of the digit to be displayed
 *	Returns: r2 = bit pattern to be written to the HEX display
 */
SEG7_CODE:  movia   r15, BIT_CODES  # starting address of bit codes
            add     r15, r15, r4    # index into the bit codes
            ldb     r2, (r15)       # read the bit code needed for our
                                    # digit
            ret

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      # pad with 2 bytes to maintain word alignment


...
code for Part III (not shown)
...
/* now display r10 on HEX1-0, r11 on HEX3-2 and r12 on HEX5-4 */
DISPLAY:    movia   r8, 0xFF200020  
            mov     r4, r10         # display r10 on HEX1??0
            call    DIVIDE          # ones digit will be in r2; tens
                                    # digit in r3
            mov     r4, r2          # pass ones digit to SEG7_CODE
            call    SEG7_CODE       
            mov     r14, r2         # save bit code
            mov     r4, r3          # retrieve tens digit, pass to
                                    # SEG7_CODE
            call    SEG7_CODE       
            slli    r2, r2, 8       
            or      r14, r14, r2    # bit code for tens j bit code for
            ...                     # ones
            code for r11 (not shown) 
            ...
            stw     r14, (r8)       # display the results from r10 and r11

            code for r12 (not shown)
            ...
