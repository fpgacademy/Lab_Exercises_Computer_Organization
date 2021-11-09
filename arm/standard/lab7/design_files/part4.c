int count       = 0; // global counter for red lights
int run         = 1; // global, used to increment/not the count variable
int time        = 0; // global, used for real-time clock
int HEX_code3_0 = 0; // global, used for 7-segment displays
int HEX_code5_4 = 0; // global, used for 7-segment displays

int main(void)
{
    volatile int * LEDR_ptr      = (int *)0xFF200000;
    volatile int * HEX3_HEX0_ptr = (int *)0xFF200020;
    volatile int * HEX5_HEX4_ptr = (int *)0xFF200030;

    set_A9_IRQ_stack();  // initialize the stack pointer for IRQ mode
    config_GIC();        // configure the general interrupt controller
    config_priv_timer(); // configure the MPCore private timer
    config_HPS_timer();  // configure HPS Timer 0
    config_KEYs();       // configure pushbutton KEYs to generate interrupts

    enable_A9_interrupts(); // enable interrupts in the A9 processor

    while (1)            // wait for an interrupt
    { 
        *LEDR_ptr      = count;
        *HEX3_HEX0_ptr = HEX_code3_0; // show the time in the format MM:SS:DD
        *HEX5_HEX4_ptr = HEX_code5_4;
    }
}

// code not shown for other subroutines

/* Set up MPCore private timer */
void config_priv_timer()
{
    // code not shown
}

// code not shown for other subroutines
