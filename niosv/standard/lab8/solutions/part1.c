#define FPGA_ONCHIP_BASE      0x08000000
#define PIXEL_BUF_CTRL_BASE   0xFF203020

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

volatile int pixel_buffer_start;
int resolution_x, resolution_y;     // VGA screen size

void get_screen_specs(void);
void wait_for_vsync(void);

void clear_screen(void);
void plot_pixel(int, int, short int);
void draw_line(int, int, int, int, int);

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;

    // set the location of the pixel buffer in the pixel buffer controller
    *(pixel_ctrl_ptr + 1) =
        FPGA_ONCHIP_BASE; // first store the address in the back buffer
    // now, swap the front and back buffers, to set the front pixel buffer
    wait_for_vsync();
    // initialize a pointer to the pixel buffer, used by drawing functions
    pixel_buffer_start = *pixel_ctrl_ptr;

    get_screen_specs();
    clear_screen();
    draw_line(0, 0, 150, 150, BLUE);
    draw_line(150, 150, 319, 0, GREEN);
    draw_line(0, 120, 319, 120, RED);
    draw_line(319, 0, 0, 239, PINK);
    return 0;
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

/* Bresenham's line drawing algorithm */
void draw_line(int x0, int y0, int x1, int y1, int color) {
/* This function draws a line between points (x0, y0) and (x1, y1) */
    char steep = (ABS(y1 - y0) > ABS(x1 - x0)) ? 1 : 0;
    int  deltax, deltay, error, ystep, x, y;

    /* Preprocessing inputs */
    if (steep > 0) {
        // Swap x0 and y0
        error = x0;
        x0 = y0;
        y0 = error;
        // Swap x1 and y1
        error = x1;
        x1 = y1;
        y1 = error;
    }
    if (x0 > x1) {
        // Swap x0 and x1
        error = x0;
        x0 = x1;
        x1 = error;
        // Swap y0 and y1
        error = y0;
        y0 = y1;
        y1 = error;
    }

    /* Setup local variables */
    deltax = x1 - x0;
    deltay = ABS(y1 - y0);
    error = -(deltax / 2);
    y = y0;
    if (y0 < y1)
        ystep = 1;
    else
        ystep = -1;

    /* Draw a line - either go along the x-axis (steep = 0) or along the y-axis
     * (steep = 1) */
    for (x = x0; x <= x1; x++) {
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
