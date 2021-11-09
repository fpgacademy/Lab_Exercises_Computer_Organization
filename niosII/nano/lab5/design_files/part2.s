                .text                               
                .global _start                      
_start:                                 
                /* set up the stack */
                ... code not shown

                call    CONFIG_TIMER    
                call    CONFIG_KEYS     

                /* enable Nios II processor interrupts */
                ... code not shown

                movia   r8, /* insert green lights LED base address */
LOOP:           ldw     r9, COUNT(r0)   # global variable 
                stw     r9, (r8)        
                br      LOOP            

/* Configure the interval timer to create interrupts at 0.25 s intervals */
CONFIG_TIMER:                           
                ... code not shown 
                ret                        

/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:                            
                ... code not shown 
                ret
                        
/* Global variables */
                .global COUNT                       
COUNT:          .word   0x0             # used by timer
                .global RUN             # used by pushbutton KEYs
RUN:            .word   0x1             # initial value to increment COUNT

                .end                                
