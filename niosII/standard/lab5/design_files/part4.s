                .text                                                       
                .global _start                                              
_start:                                                         
                /* set up the stack */
                .. code not shown 

                call    CONFIG_TIMER       
                     
                /* enable Nios II processor interrupts */
                ... code not shown 

                movia   r9, /* insert HEX3-0 base address here */ 
LOOP:           ldw     r10, HEX_code(r0)      # global variable
                stw     r10, (r9)              # show the time in format SS:DD
                br      LOOP                                    

/* Configure the interval timer to create interrupts at 1/100 second intervals */
CONFIG_TIMER:                                                   
                ... code not shown 

/* Global variables */
                .global COUNT                                               
TIME:           .word   0x0                    # used for real-time clock
                .global HEX_code                                            
HEX_code:       .word   0x0                    # used for 7-segment displays
