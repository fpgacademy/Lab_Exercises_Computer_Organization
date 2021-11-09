            .text                           
            .global _start                   
_start:                              
            /* Set up stack pointer */
            ... code not shown 

            /* Configure the pushbutton KEYs port to generate interrupts */
            ... code not shown

            /* Enable IRQ interrupts in the Nios II processor */
            ... code not shown 

MAIN_LOOP:                          
            and     r10, r11, r12   
            xor     r13, r14, r15   
            or      r16, r17, r18   
            and     r18, r17, r16   
            xor     r15, r14, r13   
            or      r12, r11, r10   
            br      MAIN_LOOP       

            .end                            
