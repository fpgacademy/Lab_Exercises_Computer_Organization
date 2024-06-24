#define LEDR_BASE 0xFF200000
#define HEX3_HEX0_BASE 0xFF200020
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050
	
static void handler(void) __attribute__ ((interrupt ("machine")));
void set_KEY(void);
void KEY_ISR(void);

/* Program that displays SW switch settings on LEDR. When a KEY is pressed,
   an interrupt is generated and the handler displays SW on HEX0 */
int main(void) {
    /* Declare volatile pointers to I/O registers (volatile means that the
     * accesses will always go to the memory (I/O) address */
    volatile int *LEDR_ptr = (int *) LEDR_BASE;
    volatile int *SW_ptr = (int *) SW_BASE;
    int SW;

    set_KEY();

    int mstatus_value, mtvec_value, mie_value;
    mstatus_value = 0b1000;   // interrupt bit mask
    // disable interrupts
    __asm__ volatile ("csrc mstatus, %0" :: "r"(mstatus_value));
    mtvec_value  = (int) &handler; // set trap address
    __asm__ volatile ("csrw mtvec, %0" :: "r"(mtvec_value));
    // disable all interrupts that are currently enabled
    __asm__ volatile ("csrr %0, mie" : "=r"(mie_value));
    __asm__ volatile ("csrc mie, %0" :: "r"(mie_value));
    mie_value = 0x40000; // KEY IRQ bit
    __asm__ volatile ("csrs mie, %0" :: "r"(mie_value));
    // enable Nios V interrupts
    __asm__ volatile ("csrs mstatus, %0" :: "r"(mstatus_value));

    while (1) {
        SW = *SW_ptr;       // read SW switches
        *LEDR_ptr = SW;     // show SW on LEDR lights
    }
}

// Trap handler: call KEY_ISR to show SW switch values on LEDR
void handler (void){
    int mcause_value;
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause_value));
    if (mcause_value == 0x80000012) // KEY IRQ number
        KEY_ISR();
    // else, ignore the trap
}

// KEY port interrupt service routine. Show SW on HEX0
void KEY_ISR(void){
    int pressed, SW;
    volatile int *HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
    volatile int *SW_ptr = (int *) SW_BASE;
    volatile int *KEY_ptr = (int *) KEY_BASE;
    pressed = *(KEY_ptr + 3);  // read Edgecapture
    *(KEY_ptr + 3) = pressed;  // clear interrupt
    SW = *SW_ptr;
    *HEX3_HEX0_ptr = SW;
}

// Configure the KEY port
void set_KEY(void){
    volatile int *KEY_ptr = (int *) KEY_BASE;
    *(KEY_ptr + 3) = 0xF; // clear Edgecapture register
    *(KEY_ptr + 2) = 0xF; // enable interrupts for all KEYs
}
