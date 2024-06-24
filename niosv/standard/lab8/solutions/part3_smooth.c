#include <stdlib.h> 

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
#define BLACK 0x0

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 6

int resolution_x, resolution_y; // VGA screen size
volatile int x_box[NUM_BOXES], y_box[NUM_BOXES];    // x, y coordinates of boxes to draw
volatile int dx_box[NUM_BOXES], dy_box[NUM_BOXES];  // amount to move boxes in animation
volatile int color_box[NUM_BOXES];                  // color
volatile int x_box_last[NUM_BOXES], y_box_last[NUM_BOXES]; // previous box coordinates 
unsigned int color[] = {WHITE, YELLOW, RED, GREEN, BLUE, CYAN, MAGENTA, GREY, PINK, ORANGE};
volatile int pixel_buffer_start;

void get_screen_specs(void);
void clear_screen(void);
void draw(void);
void draw_box(int, int, int, int, short int);
void plot_pixel(int, int, short int);
void draw_line(int, int, int, int, int);
void wait_for_vsync(void);

short int SDRAM_back_buffer[512 * 256];

/******************************************************************************
 * This program draws rectangles and boxes on the VGA screen, and moves them
 * in an animated way.
 *****************************************************************************/
int main(void)
{
    int i;
    volatile int * pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE; // pixel controller

    // initialize the location of the front pixel buffer
    *(pixel_ctrl_ptr + 1) = FPGA_ONCHIP_BASE; // first write to the back buffer
    // now, swap the front and back buffers
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    /* erase the pixel buffer */
    get_screen_specs(); // determine X, Y screen size
    clear_screen();

    // set a location for the back pixel buffer 
    *(pixel_ctrl_ptr + 1) = (int) SDRAM_back_buffer;;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen();

    // set random initial position and "delta" for all rectangle boxes
    for (i = 0; i < NUM_BOXES; i++) {
        x_box[i]  = (rand() % (resolution_x-1));    // random x position
        y_box[i]  = (rand() % (resolution_y-1));    // random y position
        dx_box[i] = (rand() % 3) - 1; // random value of 0, 1, or -1
        dy_box[i] = (rand() % 3) - 1; // random value of 0, 1, or -1
        color_box[i] = color[(rand()%10)];  // random out of 10 colors
    }

    while (1) {
        draw ();
        wait_for_vsync(); // swap the front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // update back buffer pointer
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

/* Code that draws the animation objects */
void draw(void) {
    int i;

    /* Erase any boxes and lines that may have been previously drawn */
    for (i = 0; i < NUM_BOXES; i++) {
        // Erase box
        draw_box(x_box_last[i], y_box_last[i], x_box_last[i] + BOX_LEN, y_box_last[i] + 
            BOX_LEN, BLACK);
        // Erase line that may have been drawn
        draw_line(x_box_last[i], y_box_last[i], x_box_last[(i + 1) % NUM_BOXES],
                      y_box_last[(i + 1) % NUM_BOXES], BLACK);
    }
    for (i = 0; i < NUM_BOXES; i++) {
        x_box_last[i] = x_box[i];
        y_box_last[i] = y_box[i];
        /* Change box position */
        x_box[i] += dx_box[i];
        y_box[i] += dy_box[i];

        if ((x_box[i] + BOX_LEN >= resolution_x) || (x_box[i] <= 0)) {
            dx_box[i] = -dx_box[i];
            x_box[i] += dx_box[i];
        }
        if ((y_box[i] + BOX_LEN >= resolution_y) || (y_box[i] <= 0)) {
            dy_box[i] = -dy_box[i];
            y_box[i] += dy_box[i];
        }
    }
    for (i = 0; i < NUM_BOXES; i++) {
        // Draw new box
        draw_box(x_box[i], y_box[i], x_box[i] + BOX_LEN, y_box[i] + BOX_LEN, color_box[i]);
        draw_line(x_box[i], y_box[i], x_box[(i + 1) % NUM_BOXES],
                      y_box[(i + 1) % NUM_BOXES], color_box[i]);
    }
}

void draw_box(int x1, int y1, int x2, int y2, short int color) {
    int x_ul, y_ul, x_lr, y_lr;
    int x, y;
    
    /* assume that the box coordinates are valid */
    x_ul = x2 > x1 ? x1 : x2;
    x_lr = x2 > x1 ? x2 : x1;
    y_ul = y2 > y1 ? y1 : y2;
    y_lr = y2 > y1 ? y2 : y1;
    for (x = x_ul; x <= x_lr; x++)
        for (y = y_ul; y <= y_lr; y++)
            plot_pixel (x, y, color);
}

/* Bresenham's line drawing algorithm. */
void draw_line(int x0, int y0, int x1, int y1, int color) {
/* This function draws a line between points (x0, y0) and (x1, y1). The function
 * assumes that the coordinates are valid */
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


/* This function causes a swap of the front and back buffer addressed in the VGA
 * pixel controller. The swap happens only on a vertical sync of the VGA
 * controller, which happens every is 1/60 seconds. */
void wait_for_vsync() {
   volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE; // pixel controller
   register int status;

   *pixel_ctrl_ptr = 1; // start the synchronization process

   status = *(pixel_ctrl_ptr + 3);
   while ((status & 0x01) != 0) {
       status = *(pixel_ctrl_ptr + 3);
   }
}

