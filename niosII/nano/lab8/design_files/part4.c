int main(void)
{
    config_KEYs(); // configure pushbutton KEYs to generate interrupts
    
    enable_nios2_interrupts(); // enable interrupts in the Nios II processor

    while (1) // wait for an interrupt
    {
        asm("and r10, r11, r12");
        asm("xor r13, r14, r15");
        asm("or r16, r17, r18");
        asm("and r18, r17, r16");
        asm("xor r15, r14, r13");
        asm("or r12, r11, r10");
    }
}

/* Set up the pushbutton KEYs port in the FPGA */
void config_KEYs(void)
{
    // code not shown
}
/* Turn on interrupts in the Nios II processor */
void enable_nios2_interrupts(void)
{
    // code not shown
}
