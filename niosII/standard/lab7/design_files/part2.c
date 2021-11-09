int count = 0; // global counter for red lights
int run   = 1; // global, used to increment/not the count variable

int main(void)
{
    volatile int * LED_ptr = (int *) 0xFF200000;

    config_timer();            // configure interval timer
    config_KEYs();             // configure pushbutton KEYs to generate
                               // interrupts
    enable_nios2_interrupts(); // enable interrupts in the Nios II processor
    
    while (1)                  // wait for an interrupt
        *LED_ptr = count;
}

/* Set up timer */
void config_timer()
{
    // code not shown
}
/* Set up the pushbutton KEYs port in the FPGA */
void config_KEYs()
{
    // code not shown
}
/* Turn on interrupts in the Nios II processor */
void enable_nios2_interrupts()
{
    // code not shown
}
