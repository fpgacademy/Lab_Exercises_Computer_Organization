            .text
            .global _start
_start:     # set up stack location
            ... code |not| shown
            jal     set_timer           # initialize the timer
            jal     setup_KEY           # configure the KEY port
     
            # enable interrupts for Machine Timer and KEY port
            ... code |not| shown
            .
            .

            la      s0, counter         # pointer to counter
            li      s1, LEDR_BASE       # pointer to red lights
loop:       wfi
            lw      a0, (s0)            # load the counter value
            sw      a0, (s1)            # write to the lights
            j       loop

# Trap handler
handler:    ... code |not| shown
            .
            .
            mret

# Set Machine Timer for 1/4 second timeout
set_timer:  ... code |not| shown
            .
            .
            ret

# Enable interrupts in the KEY port
setup_KEY:  ... code |not| shown
            .
            .
            ret  

            .data
counter:    .word   0                   # the counter to be displayed
run:        .word   1                   # the amount to be added
