int main(void) 
{
    set_A9_IRQ_stack();     // initialize the stack pointer for IRQ mode
    config_GIC();           // configure the general interrupt controller
    config_KEYs();          // configure pushbutton KEYs to generate
                            // interrupts
    enable_A9_interrupts(); // enable interrupts in the A9 processor
    
    while (1)               // wait for an interrupt
        ;
}

/* Initialize the banked stack pointer register for IRQ mode */
void set_A9_IRQ_stack(void) 
{
    // code not shown
}

/* Configure the Generic Interrupt Controller (GIC) */
void config_GIC(void) 
{
    // code not shown
}

/* Set up the pushbutton KEYs port in the FPGA */
void config_KEYs(void) 
{
    // code not shown
}

/* Turn on interrupts in the ARM processor */
void enable_A9_interrupts(void) 
{
    // code not shown
}
