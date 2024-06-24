static void handler(void) __attribute__ ((interrupt ("machine")));
void set_mtimer(void);
void set_KEY(void);
void mtimer_ISR(void);

volatile int time = 0;    // binary counter to be displayed
volatile int flag = 0;    // run/pause the clock

// 7-segment display bit code for digits 0 to 9
char seg7[10] =  {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 
                 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};

// Display a real-time clock on the 7-segment displays.
int main(void) {
    ... declare variables (code not shown)
    .
    .
    set_mtimer();
    set_KEY();

    // enable interrupts for Machine Timer and KEY port
    ... code not shown
    .
    .
    while (1) {
        // check for KEY pressed (code not shown)
        .
        .
        MM = time / 6000;   // display the time
        SS = ...
        DD = ...
        *HEX5_HEX4_ptr = ...
        *HEX3_HEX0_ptr = ...
    }
}

// Trap handler
void handler (void){
    ... code not shown
    .
    .
}

... other required subroutines not shown
.
.
