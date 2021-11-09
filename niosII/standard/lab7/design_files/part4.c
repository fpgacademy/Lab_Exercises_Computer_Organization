int time           = 0; // global, used for real-time clock
int HEX_code_lower = 0; // global, used for 7-segment displays
int HEX_code_upper = 0; // global, used for 7-segment displays

int main(void)
{
    volatile int * HEX_lower_ptr = (int *) // insert HEX3_HEX0 base address;
    volatile int * HEX_upper_ptr = (int *) // insert HEX7_HEX4/HEX5_HEX4
                                           // base address here;
    config_timer();            // configure the Interval Timer

    enable_nios2_interrupts(); // enable interrupts in the Nios II processor

    while (1)                  // wait for an interrupt
    { 
        *LEDR_ptr      = count;
        *HEX_lower_ptr = HEX_code_lower; // show the time in the format
                                         // MM:SS:DD
        *HEX_upper_ptr = HEX_code_upper;
    }
}

/* Set up the Interval Timer */
void config_timer()
{
    // code not shown
}
