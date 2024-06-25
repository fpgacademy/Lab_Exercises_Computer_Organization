.global _start
_start:     # set up stack location
            ... code |not| shown
            jal     set_timer           # initialize the timer
     
            # enable interrupts for Machine Timer
            ... code |not| shown
            .
            .

            la      s0, HEX_code        # pointer to variable
            la      s1, HEX3_HEX0_BASE  # pointer to displays
            la      s2, SW_BASE         # pointer to switches
loop:       wfi
            lw      t0, (s0)            # load the counter value
            sw      t0, (s1)            # write to the lights
wait:       lw      t0, (s2)            # if any SW is set, wait
            bnez    t0, wait

            j       loop

# Trap handler
handler:    ... code |not| shown
            .
            .
            mret

# Set Machine Timer for 1/100 second timeout
set_timer:  ... code |not| shown
            .
            .
            ret

# Convert the time value to codes for display on HEX1-0; store in HEX_code
display:    ... code |not| shown
            .
            .
            ret

# Subroutine to convert the digits from 0 to 9 to be shown on a HEX display.
# Parameters: a0 = the decimal digit to be displayed
# Returns: a0 = bit pattern to be written to the HEX display
seg7_code:  la      t0, bit_codes       # starting address of the bit codes
            add     t0, t0, a0          # index into the bit codes
            lb      a0, (t0)            # load the bit code
            ret

time:       .word   0                   # the time value
HEX_code:   .word   0                   # the display codes
# 7-segment codes for digits 0, 1, ..., 9
bit_codes:  .byte   0x3f, 0x06, 0x5b, 0x4f, 0x66
            .byte   0x6d, 0x7d, 0x07, 0x7f, 0x67
