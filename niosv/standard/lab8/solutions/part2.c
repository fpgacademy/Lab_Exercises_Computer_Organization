#define SDRAM_BASE            0x00000000
#define FPGA_ONCHIP_BASE      0x08000000
#define FPGA_CHAR_BASE        0x09000000

#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* This program implements a basic animation by drawing a horizontal line that
 * moves up and down the screen. The speed of movement is controlled by the VGA
 * vertical sync timing */

int resolution_x, resolution_y;     // VGA screen size

volatile int pixel_buffer_start;

void wait_for_vsync(void);
void get_screen_specs(void);
void clear_screen(void);
void draw_line(int, int, int, int, int);
void plot_pixel(int, int, short int);

int main(void)
{
    int x_0, y, x_1, y_dir;
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE; // pixel controller

    // initialize the location of the pixel buffer in the pixel buffer
    *(pixel_ctrl_ptr + 1) = FPGA_ONCHIP_BASE; // first write to the back buffer
    // now, swap the front and back buffers
    wait_for_vsync();
    // initialize a pointer to the pixel buffer, used by drawing functions
    pixel_buffer_start = *pixel_ctrl_ptr;

    get_screen_specs();
    clear_screen(); // blank the VGA screen

    /* initialize location of the back pixel buffer in the pixel buffer controller */
    *(pixel_ctrl_ptr + 1) = FPGA_ONCHIP_BASE;

    x_0 = 100;
    x_1 = 220;
    y = 0;
    y_dir = 1;

    draw_line(x_0, y, x_1, y, WHITE);

    while (1) {
        // Wait for redraw cycle
        wait_for_vsync();
        // Erase old line
        draw_line(x_0, y, x_1, y, 0);   // draw in black
        // Change line position
        y = y + y_dir;

        if ((y <= 0) || (y >= 239))
            y_dir = -y_dir;
        // Draw new line
        draw_line(x_0, y, x_1, y, WHITE);
    }
}

// This function causes a swap of the front and back buffers.
void wait_for_vsync() {
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE; // pixel controller
    register int status;

    *pixel_ctrl_ptr = 1; // start the synchronization process

    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0) {
        status = *(pixel_ctrl_ptr + 3);
    }
}

/* Function to find the VGA screen specifications */
void get_screen_specs() {
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
    int resolution_reg;

    resolution_reg = *(pixel_ctrl_ptr + 2);
    resolution_x   = resolution_reg & 0xFFFF;
    resolution_y   = resolution_reg >> 16;
}

/* Function to blank the VGA screen */
void clear_screen() {
	int y, x;

	for (x = 0; x < resolution_x; x++)
		for (y = 0; y < resolution_y; y++)
			plot_pixel (x, y, 0);
}

/* Bresenham's line drawing algorithm.  This function draws a line between
 * points (x0, y0) and (x1, y1). The function assumes that the coordinates are
 * valid */
void draw_line(int x0, int y0, int x1, int y1, int color) {
    register int  x_0   = x0;
    register int  y_0   = y0;
    register int  x_1   = x1;
    register int  y_1   = y1;
    register char steep = (ABS(y_1 - y_0) > ABS(x_1 - x_0)) ? 1 : 0;
    register int  deltax, deltay, tmp, error, ystep, x, y;

    /* Preprocessing inputs */
    if (steep > 0) {
        // Swap x_0 and y_0
        tmp = x_0;
        x_0 = y_0;
        y_0 = tmp;
        // Swap x_1 and y_1
        tmp = x_1;
        x_1 = y_1;
        y_1 = tmp;
    }
    if (x_0 > x_1) {
        // Swap x_0 and x_1
        tmp = x_0;
        x_0 = x_1;
        x_1 = tmp;
        // Swap y_0 and y_1
        tmp = y_0;
        y_0 = y_1;
        y_1 = tmp;
    }

    /* Setup local variables */
    deltax = x_1 - x_0;
    deltay = ABS(y_1 - y_0);
    error  = -(deltax / 2);
    y = y_0;
    if (y_0 < y_1)
        ystep = 1;
    else
        ystep = -1;

    /* Draw a line - either go along the x-axis (steep = 0) or along the y-axis
     * (steep = 1) */
    for (x = x_0; x <= x_1; x++) {
        if (steep == 1)
            plot_pixel(y, x, color);
        else
            plot_pixel(x, y, color);
        error = error + deltay;
        if (error > 0) {
            y = y + ystep;
            error = error - deltax;
        }
    }
}

void plot_pixel(int x, int y, short int line_color) {
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}
