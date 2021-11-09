#include <stdio.h>

/* Function prototypes */
int  KEY_pressed(void);
void doit(void);

/* This program demonstrates the use of inline assembly code in C code */
int main(void) 
{
    while (1) // endless loop
    {
        if (KEY_pressed()) doit();
        asm("and r10, r11, r12");
        if (KEY_pressed()) doit();
        asm("xor r13, r14, r15");
        if (KEY_pressed()) doit();
        asm("or r16, r17, r18");
        if (KEY_pressed()) doit();
        asm("and r18, r17, r16");
        if (KEY_pressed()) doit();
        asm("xor r15, r14, r13");
        if (KEY_pressed()) doit();
        asm("or r12, r11, r10");
        if (KEY_pressed()) doit();
    }
}

int KEY_pressed() 
{
    // code not shown
}

void doit() 
{
    unsigned int machine_code;
    // get the machine code of the next instruction on return from this
    // subroutine
    asm("ldw r10, 0(ra)" : : : "r10");            // read machine code into R0
    asm("mov %0, r10" : "=r"(machine_code) : :);  // copy R0 into variable
                                                  // machine code
    // code not shown
}
