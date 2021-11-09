#include "nios2_ctrl_reg_macros.h"

/* function prototypes */
void main(void);
void interrupt_handler(void);
void pushbutton_ISR(void);

/* The assembly language code below handles CPU reset processing */
void the_reset(void) __attribute__((section(".reset")));
void the_reset(void)
/****************************************************************************
 * Reset code. By giving the code a section attribute with the name ".reset"
 * we allow the linker program to locate this code at the proper reset
 * vector address.
 * This code just calls the main program. 
 ***************************************************************************/
{
    asm(".set		noat");     // Magic, for the C compiler
    asm(".set		nobreak");  // Magic, for the C compiler
    asm("movia		r2, main"); // Call the C language main program
    asm("jmp		r2");
}

/* The assembly language code below handles CPU exception processing. This
 * code should not be modified; instead, the C language code in the function
 * interrupt_handler() can be modified as needed for a given application.
*/
void the_exception(void) __attribute__((section(".exceptions")));
void the_exception(void)
/****************************************************************************
 * Exceptions code. By giving the code a section attribute with the name    *
 * ".exceptions" we allow the linker program to locate this code at the     *
 * proper exceptions vector address.                                        *
 * This code calls the interrupt handler and later returns from the 	    *
 * exception.								    *
 ***************************************************************************/
{
    asm(".set		noat");    // Magic, for the C compiler
    asm(".set		nobreak"); // Magic, for the C compiler
    asm("subi		sp, sp, 128");
    asm("stw		et, 96(sp)");
    asm("rdctl	et, ctl4");
    asm("beq		et, r0, SKIP_EA_DEC"); // Interrupt is not external
    asm("subi		ea, ea, 4"); // Must decrement ea by one instruction for
                                 // external interupts, so that the interrupted
                                 // instruction will be run

    asm("SKIP_EA_DEC:");
    asm("stw	r1,  4(sp)"); // Save all registers
    asm("stw	r2,  8(sp)");
    // ...
    // ... save all regs, except for r27 (sp)
    // ...
    asm("stw	r31, 124(sp)"); // r31 = ra
    asm("addi	fp,  sp, 128");

    asm("call	interrupt_handler"); // Call the C language interrupt handler

    asm("ldw	r1,  4(sp)"); // Restore all registers
    asm("ldw	r2,  8(sp)");
    // ...
    // ... restore all saved regs
    // ...
    asm("ldw	r31, 124(sp)"); // r31 = ra

    asm("addi	sp,  sp, 128");

    asm("eret");
}

/*****************************************************************************
 * Interrupt Service Routine
 *  Determines what caused the interrupt and calls the appropriate
 *  subroutine.
 *
 * ipending - Control register 4 which has the pending external interrupts
 *****************************************************************************/
void interrupt_handler(void)
{
    int ipending;
    NIOS2_READ_IPENDING(ipending);
    if (ipending & 0x2) // pushbuttons are interrupt level 1
    {
        pushbutton_ISR();
    }
    // else, ignore the interrupt
    return;
}

