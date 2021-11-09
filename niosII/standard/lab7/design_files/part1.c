#include "nios2_ctrl_reg_macros.h"

int main(void)
{
    config_KEYs();             // configure pushbutton KEYs to generate
                               // interrupts
    enable_nios2_interrupts(); // enable interrupts in the Nios II processor

    while (1)                  // wait for an interrupt
        ;
}

/* Set up the pushbutton KEYs port in the FPGA */
void config_KEYs(void)
{
    // code not shown
}

/* Enable interrupts in the Nios II processor */
void enable_nios2_interrupts(void)
{
    // code not shown
}
