# This program displays a selected save register on the Terminal window

.equ SW_BASE, 0xFF200040
.equ KEY_BASE, 0xFF200050
.equ JTAG_UART_BASE, 0xFF201000

.global _start 
_start:     li      sp, 0x20000         # set up the stack
            la      a0, prompt          # print a prompt
            jal     puts
            la      s0, SW_BASE
            la      s1, KEY_BASE

get_KEY:    ... wait for a KEY; code |not| shown
            .
            .
            ... print the string "Contents of s"
            ... then print the SW index
            .
            .
            jal     get_reg             # pass a0 (SW) to get_reg
            .
            .
            .
            ... call either hex_word or dec_word
            .
            .

next:       li      a0, '\n'
            jal     put_JTAG

            j       get_KEY

            .
            .
            .
prompt:     .asciz  "Press a pushbutton KEY to continue...\n" 
string:     .asciz  "Contents of s" 
