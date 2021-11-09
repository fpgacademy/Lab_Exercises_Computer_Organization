volatile int pixel_buffer_start; // global variable

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();
    draw_line(0, 0, 150, 150, 0x03);   // this line is blue
    draw_line(150, 150, 319, 0, 0x1C); // this line is green
    draw_line(0, 239, 319, 239, 0xE0); // this line is red
    draw_line(319, 0, 0, 239, 0xED);   // this line is a pink color
}

// code not shown for clear_screen() and draw_line() subroutines

void plot_pixel(int x, int y, char line_color)
{
    *(char *)(pixel_buffer_start + (y << 9) + x) = line_color;
}
