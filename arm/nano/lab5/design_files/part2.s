                  .section .vectors, "ax"                  
                  ... code not shown

                  .text                                       
                  .global  _start                          
_start:                                         
/* Set up stack pointers for IRQ and SVC processor modes */
                  ... code not shown

                  BL       CONFIG_GIC          // configure the ARM generic
                                               // interrupt controller
                  BL       CONFIG_HPS_TIMER    // configure HPS Timer 0
                  BL       CONFIG_KEYS         // configure the pushbutton
                                               // KEYs port

/* Enable IRQ interrupts in the ARM processor */
                  ... code not shown
                  LDR      R5, =0xFF200000     // LED base address
LOOP:                                            
                  LDR      R3, COUNT           // global variable
                  STR      R3, [R5]            // light up the green lights
                  B        LOOP                

/* Configure the HPS timer to create interrupts at 0.25 second intervals */
CONFIG_HPS_TIMER:                             
                  ... code not shown
                  BX       LR                  

/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:                                    
                  ... code not shown
                  BX       LR                  

/* Global variables */
                  .global  COUNT                           
COUNT:            .word    0x0             // used by timer
                  .global  RUN             // used by pushbutton KEYs
RUN:              .word    0x1             // initial value to increment
                                           // COUNT
                  .end                                        
