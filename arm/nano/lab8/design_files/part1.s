        .section    .vectors, "ax" 
        .word       0           // reset vector
        .word       0           // undefined instruction vector
        .word       0           // software interrrupt vector
        .word       0           // aborted prefetch vector
        .word       0           // aborted data vector
        .word       0           // unused vector
        B           SERVICE_IRQ // IRQ interrupt vector
        .word       0           // FIQ interrupt vector

        .text        
        .global     _start 
_start:                      
        /* Set up stack pointers for IRQ and SVC processor modes */
        ... code not shown 
        BL       CONFIG_GIC  // configure the ARM generic interrupt
                             // controller

        /* Configure the pushbutton KEYs port to generate interrupts */
        ... code not shown
        /* Enable IRQ interrupts in the ARM processor */
        ... code not shown
MAIN_LOOP:                   
        AND      R0, R1, R2  
        EOR      R3, R4, R5  
        ORR      R6, R7, R8  
        AND      R8, R7, R6  
        EOR      R5, R4, R3  
        ORR      R2, R1, R0  
        B        MAIN_LOOP   

        .end         
