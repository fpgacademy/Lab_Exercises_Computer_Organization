/* Define the IRQ exception handler */
SERVICE_IRQ:    PUSH    {R0-R7, LR}     

                LDR     R4, =0xFFFEC100 // GIC CPU interface base address
                LDR     R5, [R4, #0x0C] // read the ICCIAR in the CPU
                                        // interface

CHECK_KEYS:     CMP     R5, #73         // check the interrupt ID
UNEXPECTED:     BNE     UNEXPECTED      // if not recognized, stop here

                /* load into register R0 the machine code of the instruction 
                   that will be executed on return from this interrupt service
                   routine */
                ... code not shown
                BL      KEY_ISR         

EXIT_IRQ:       STR     R5, [R4, #0x10] // write to the End of Interrupt
                                        // Register

                POP     {R0-R7, LR}     
                SUBS    PC, LR, #4      // return from exception

                .end                                
