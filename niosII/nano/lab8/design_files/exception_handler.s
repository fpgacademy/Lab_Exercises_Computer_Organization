/************************* RESET SECTION ***********************************/
            .section .reset, "ax"            
            movia    r2, start               
            jmp      r2                      # branch to main program
/************************* EXCEPTIONS SECTION ******************************/
            .section .exceptions, "ax" 
            .global  EXCEPTION_HANDLER 
EXCEPTION_HANDLER:                           
            subi     sp, sp, 20              # make room on the stack
            stw      et, 0(sp)               
            rdctl    et, ctl4                
            beq      et, r0, SKIP_EA_DEC     # interrupt is not external
            subi     ea, ea, 4               # must decrement ea by one 
                                             # instruction for external 
                                             # interrupts, so the interrupted 
                                             # instruction will be re-run
SKIP_EA_DEC:                                 
            stw      ea, 4(sp)               # save all used registers on the 
                                             # Stack
            stw      ra, 8(sp)               # needed if call inst is used
            stw      r4, 12(sp)              
            stw      r22, 16(sp)      
        
            rdctl    et, ctl4                
            bne      et, r0, CHECK_LEVEL_1   # interrupt is an external
                                             # interrupt
NOT_EI:     br       END_ISR                 # must be unimplemented
                                             # instruction or TRAP instruction;
                                             # ignored in this code 
CHECK_LEVEL_1:                               # pushbutton port is interrupt 
                                             # level 1
            andi     r22, et, 0b10           
            beq      r22, r0, END_ISR        # other interrupt levels not 
                                             # handled in this code
            /* load into r4 the machine code of the instruction that will */
            /* be executed on the return from hsi interrupt service routine */
            call     KEY_ISR     
            
END_ISR:    ldw      et, 0(sp)               # restore all used register to
                                             # prev values
            ldw      ea, 4(sp)               
            ldw      ra, 8(sp)               # needed if call inst is used
            ldw      r4, 12(sp)              
            ldw      r22, 16(sp)             
            addi     sp, sp, 20              

            .end         
