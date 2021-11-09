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
        asm("AND R0, R1, R2");
        if (KEY_pressed()) doit();
        asm("EOR R3, R4, R5");
        if (KEY_pressed()) doit();
        asm("ORR R6, R7, R8");
        if (KEY_pressed()) doit();
        asm("AND R8, R7, R6");
        if (KEY_pressed()) doit();
        asm("EOR R5, R4, R3");
        if (KEY_pressed()) doit();
        asm("ORR R2, R1, R0");
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
    asm("LDR R0, [LR]" : : : "r0");              // read machine code into R0
    asm("MOV %0, R0" : "= r"(machine code) : :); // copy R0 into variable 
                                                 // machine code

    // code not shown
}
