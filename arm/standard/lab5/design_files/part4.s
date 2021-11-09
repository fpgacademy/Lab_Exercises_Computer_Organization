                    .text                                       
                    .global _start                              
_start:                                         
/* Set up stack pointers for IRQ and SVC processor modes */
                    ... code not shown
                    BL      CONFIG_GIC          // configure the ARM generic
                                                // interrupt controller
                    BL      CONFIG_PRIV_TIMER   // configure the private timer
                    BL      CONFIG_HPS_TIMER    // configure HPS Timer 0
                    BL      CONFIG_KEYS         // configure the pushbutton
                                                // KEYs port
/* Enable IRQ interrupts in the ARM processor */
                    ... code not shown
                    LDR     R5, =0xFF200000     // LEDR base address
                    LDR     R6, =0xFF200020     // HEX3-0 base address
LOOP:                                           
                    LDR     R4, COUNT           // global variable
                    STR     R4, [R5]            // light up the red lights
                    LDR     R4, HEX_code        // global variable
                    STR     R4, [R6]            // show the time in format
                                                // SS:DD
                    B       LOOP                

/* Configure the MPCore private timer to create interrupts every 1/100 seconds */
CONFIG_PRIV_TIMER:                              
                    ... code not shown
                    BX      LR                  
/* Configure the HPS timer to create interrupts at 0.25 second intervals */
CONFIG_HPS_TIMER:                               
                    ... code not shown
                    BX      LR                  
/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:                                    
                    ... code not shown
                    BX      LR                  

/* Global variables */
                    .global COUNT                               
COUNT:              .word   0x0       // used by timer
                    .global RUN       // used by pushbutton KEYs
RUN:                .word   0x1       // initial value to increment COUNT
                    .global TIME                                
TIME:               .word   0x0       // used for real-time clock
                    .global HEX_code                            
HEX_code:           .word   0x0       // used for 7-segment displays

                    .end                                        
