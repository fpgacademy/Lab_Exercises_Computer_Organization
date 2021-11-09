volatile int pixel_buffer_start;            // global variable
short int    front_pixel_buffer[512 * 256]; // allocate memory for front 
                                            // buffer
short int    back_pixel_buffer[512 * 256];  // allocate memory for back buffer
int          main(void) {
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
    /* initialize the location of the front pixel buffer in the pixel buffer
       controller */
    *(pixel_ctrl_ptr + 1) = front_pixel_buffer; // first store the address in
                                                // the back buffer
    /* now, swap the front and back buffers, to initialize front pixel buffer
     * location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel buffer start points to the pixel buffer

    /* Set a location for the pixel back buffer in the pixel buffer controller
     */
    *(pixel_ctrl_ptr + 1) = back_pixel_buffer;
    pixel_buffer_start    = *(pixel_ctrl_ptr + 1); // we draw on the back
                                                   // buffer
    while (1) {
        /* Erase any boxes and lines that were drawn in the last iteration */
        clear screen(); // pixel buffer start points to the pixel buffer

        // code for drawing the boxes and lines (not shown)
        // code for updating the locations of boxes (not shown)

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // update back buffer
                                                    // pointer
    }
}

// code for subroutines (not shown)
