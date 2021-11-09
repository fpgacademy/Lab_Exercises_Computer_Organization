        .text       
        .global _start
_start:         
        /* set up the stack */
        ... code not shown

        /* write to the pushbutton port interrupt mask register */
        ... code not shown 
	
        /* enable Nios II processor interrupts */
        ... code not shown 

IDLE:   br  IDLE	/* main program simply idles */
        .end        
