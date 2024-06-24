static void handler(void) __attribute__ ((interrupt ("machine")));
void set_mtimer(void);
void set_KEY(void);
void mtimer_ISR(void);
void KEY_ISR(void);

volatile int counter = 0;    // binary counter to be displayed
volatile int run = 1;        // the amount to be added

// Program that displays a binary counter on LEDR using interrupts
int main(void){
    volatile int *LEDR_ptr = (int *) LEDR_BASE;
    set_mtimer();
    set_KEY();

    // enable interrupts for Machine Timer and KEY port
    ... code not shown
    .
    .
    while (1) {
        *LEDR_ptr = counter;
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
