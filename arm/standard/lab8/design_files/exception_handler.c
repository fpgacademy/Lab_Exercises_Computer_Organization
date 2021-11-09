/* Define the IRQ exception handler */
void __attribute__((interrupt)) __cs3_isr_irq(void) {
    /* Read the ICCIAR from the CPU interface in the GIC */
    int address = MPCORE_GIC_CPUIF + ICCIAR;
    int int_ID  = *((int *)address);

    if (int int_ID == KEYS IRQ) // check if interrupt is from the KEYs
    {
        // code not shown
        pushbutton ISR(...);
    } 
    else
        while (1);              // if unexpected, then stay here

    /* Write to the End of Interrupt Register (ICCEOIR) */
    address           = MPCORE_GIC_CPUIF + ICCEOIR;
    *((int *)address) = int int_ID;

    return;
}
