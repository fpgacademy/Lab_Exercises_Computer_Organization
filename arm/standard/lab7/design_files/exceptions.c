/* Define the IRQ exception handler */
void __attribute__((interrupt)) __cs3_isr_irq(void) 
{
    /* Read the ICCIAR from the CPU interface in the GIC */
    int address = MPCORE_GIC_CPUIF + ICCIAR;
    int int_ID  = *((int *)address);

    if (int_ID == KEYS_IRQ) // check if interrupt is from the KEYs
        pushbutton_ISR();
    else
        while (1);          // if unexpected, then stay here

    /* Write to the End of Interrupt Register (ICCEOIR) */
    address = MPCORE_GIC_CPUIF + ICCEOIR;
    *((int *)address) = int_ID;

    return;
}

/* Define the remaining exception handlers */
void __attribute__((interrupt)) __cs3_reset(void) 
{
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_undef(void) 
{
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_swi(void) 
{
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_pabort(void) 
{
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_dabort(void) 
{
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_fiq(void) 
{
    while (1);
}
