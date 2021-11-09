/***********************RESET_SECTION****************************************/
        .section .reset, "ax"            
        movia    r2, _start              
        jmp      r2                      # branch to main program

/***********************EXCEPTIONS_SECTION***********************************/
        .section .exceptions, "ax" 
        .global  EXCEPTION_HANDLER 
EXCEPTION_HANDLER:                       
        subi     sp, sp, 16              # make room on the stack 
        stw      et, 0(sp)               
        rdctl    et, ctl4                
        beq      et, r0, SKIP_EA_DEC     # interrupt is not external
        subi     ea, ea, 4               # must decrement ea by one  
                                         # instruction for external 
                                         # interrupts, so that the interrupted 
                                         # instruction will be run 
SKIP_EA_DEC:                             
        stw      ea, 4(sp)               # save all used registers on the 
                                         # Stack 
        stw      ra, 8(sp)               # needed if call inst is used
 
        stw      r22, 12(sp)             
        rdctl    et, ctl4                
        bne      et, r0, CHECK_LEVEL_1   # interrupt is an external interrupt 

NOT_EI:                                  # must be unimplemented instruction
        br       END_ISR                 # or TRAP instruction; ignored in 
                                         # this code
CHECK_LEVEL_1:                           # pushbutton port is interrupt level
                                         # 1
        andi     r22, et, 0b10           
        beq      r22, r0, END_ISR        # other interrupt levels not handled  
                                         # in this code 
        call     KEY_ISR                 

END_ISR:                                 
        ldw      et, 0(sp)               # restore all used register to 
                                         # previous values 
        ldw      ea, 4(sp)               
        ldw      ra, 8(sp)               # needed if call inst is used 
        ldw      r22, 12(sp)             
        addi     sp, sp, 16    
        eret          

        .end         
