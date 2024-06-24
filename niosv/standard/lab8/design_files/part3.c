volatile int pixel_buffer_start;        // global variable
// declare other variables (code not shown)
...
.
.
short int SDRAM_back_buffer[512 * 256]; // space for pixel buffer

int main(void) {
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables (not shown)
    ...
    .
    .
    // initialize locations and directions of boxes (not shown)
    ...
    .
    .
    // set location of the front pixel buffer
    *(pixel_ctrl_ptr + 1) = 0x08000000; // FPGA on-chip memory
    // now, swap the back/front buffers to set the front buffer
    wait_for_vsync();

    // set pointer to pixel buffer, used by drawing functions
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // uses pixel_buffer_start

    // set location of back buffer 
    *(pixel_ctrl_ptr + 1) = SDRAM_back_buffer;  // location in SDRAM memory
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // draw on the back buffer

    while (1) {
        // erase the screen
        clear screen(); 

        // code for drawing the boxes and lines (not shown)
        ...
        .
        .
        // code for updating the locations of boxes (not shown)
        ...
        .
        .
        wait_for_vsync(); // swap back/front buffers on VGA sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // update pointer
    }
}

// code for subroutines (not shown)
...
.
.
