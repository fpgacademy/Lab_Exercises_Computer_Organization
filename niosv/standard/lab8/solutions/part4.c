/* This files provides address values that exist in the system */

#define SDRAM_BASE            0x00000000
#define FPGA_ONCHIP_BASE      0x08000000
#define FPGA_CHAR_BASE        0x09000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define MTIME_BASE            0xFF202100

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x041F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00
#define BLACK 0x0

#define KEY0 0b0001
#define KEY1 0b0010
#define KEY2 0b0100
#define KEY3 0b1000

#define FASTER 1
#define SLOWER 2

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 5         // default number of boxes to draw
#define NUM_BOXES_MAX 25    // maximum number of boxes to draw
#define MIN_LOAD 3333333    // minimum timer load value
#define MAX_SHIFT 5         // used when speeding up the animation
#define FALSE 0
#define TRUE                    

#define RESOLUTION_X 320
#define RESOLUTION_Y 240

#define TIMEOUT 100000000 / 100    // for Machine Timer

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int resolution_x, resolution_y; // VGA screen size
int sizeof_pixel;               // number of bytes per pixel
int video_m, video_n;           // number of bits in VGA y coord (m), x coord (n)
int shift_x, shift_y;           // used by plot_pixel
volatile int x_box[NUM_BOXES_MAX], y_box[NUM_BOXES_MAX];    // x, y box coordinates 
volatile int dx_box[NUM_BOXES_MAX], dy_box[NUM_BOXES_MAX];  // amount to move boxes in animation
volatile int x_box_last[NUM_BOXES_MAX], y_box_last[NUM_BOXES_MAX];  // last x, y box coordinates 
volatile int color_box[NUM_BOXES_MAX];                              // color
unsigned int color[] = {WHITE, YELLOW, RED, GREEN, BLUE, CYAN, MAGENTA, GREY, PINK, ORANGE};
int char_x, char_y; // columns and rows of text
int char_m, char_n; // number of bits in char buffer y coord (m), x coord (n)

volatile int shifted_dx_dy = 0;             // used to speed up/down animation
volatile int boxes_to_draw = NUM_BOXES;     // # of boxes to draw
volatile int boxes_to_draw_new = NUM_BOXES; // # of boxes to draw
volatile int boxes_to_draw_last;            // needed when erasing boxes/lines
volatile int no_lines      = FALSE;         // draw lines between boxes?
volatile int pixel_buffer_start;
volatile int char_buffer_start;             // start of current character buffer

void plot_pixel(int, int, short int);
void config_timer(void);
int  timer_expired(void);
void pushbutton(void);
void get_screen_specs(void);
void get_char_specs(void);
void clear_screen(void);
void erase_chars(void);
void wait_for_vsync(void);
void draw(void);
void draw_char(int, int, char);
void draw_str(int, int, char *);
void my_itoa(int, char *);
void reverse(char *);
void draw_box(int, int, int, int, short int);
void draw_line(int, int, int, int, int);
void reconfig_timer(int);

short int SDRAM_back_buffer[512 * 256];

/******************************************************************************
 * This program draws boxes and lines on the VGA screen, and moves them
 * in an animated way. The animation can be controlled using KEYs:
 *    KEY0: increases the animation speed
 *    KEY1: decreases the animation speed
 *    KEY2: removes a box
 *    KEY3: adds a box
 * Also, if all SW switches are in the down position then lines are drawn
 * to connect the boxes. Otherwise, only boxes are drawn
 *****************************************************************************/
int main(void)
{
    int i;
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE; // pixel controller
    volatile int * char_ctrl_ptr = (int *)CHAR_BUF_CTRL_BASE;   // character controller
    volatile int * SW_ptr = (int *)SW_BASE;
    char msg_buffer[80];
    int frames = 0;                                             // number of frames drawn

    config_timer(); // configure the timer

    /* initialize the location of the front pixel buffer in the pixel buffer controller */
    *(pixel_ctrl_ptr + 1) = FPGA_ONCHIP_BASE; // first store the address in the back buffer
    /* now, swap the front and back buffers, to initialize front pixel buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    // printf ("Front pixel buffer: %0X\n", pixel_buffer_start);
    get_screen_specs(); // determine X, Y screen size
    clear_screen();     // clear the pixel front buffer

    /* set a location for the back pixel buffer in the pixel buffer controller */
    *(pixel_ctrl_ptr + 1) = (int) SDRAM_back_buffer;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    // printf ("Back pixel buffer: %0X\n", pixel_buffer_start);
    // printf ("Pixel address x width: %d\n", video_n);
    clear_screen();     // clear the pixel front buffer

    /* initialize a pointer to the character buffer, used to draw text */
    char_buffer_start = *char_ctrl_ptr;
     
    // printf ("Text buffer: %0X\n", char_buffer_start);
    get_char_specs( );  // determine X, Y text size
    // char_x = 80; char_y = 60;
    // printf ("Text address x width: %d\n", char_n);
    erase_chars( );     // clear all text from the screen

    // set random initial position and "delta" for all rectangle boxes
    for (i = 0; i < NUM_BOXES_MAX; i++) {
        x_box[i]  = (rand()%(resolution_x - 2 * BOX_LEN)) + BOX_LEN;    // random x position
        y_box[i]  = (rand()%(resolution_y - 2 * BOX_LEN)) + BOX_LEN;    // random y position
        dx_box[i] = (rand() % 3) - 1; // random value of 0, 1, or -1
        dy_box[i] = (rand() % 3) - 1; // random value of 0, 1, or -1
        color_box[i] = color[(rand()%10)];  // random out of 10 colors
    }

    while (1) {
        no_lines = *SW_ptr; // allow user to turn off line drawing
        pushbutton();
        if (timer_expired()) {
            draw();
            ++frames;
            wait_for_vsync(); // swap the front and back buffers on VGA vertical sync
            pixel_buffer_start = *(pixel_ctrl_ptr + 1); // update back buffer pointer
            //sprintf (msg_buffer, "Frames rendered: %d", frames);
            draw_str(0, 0, "Frames rendered: ");
            my_itoa (frames, msg_buffer);
            draw_str(17, 0, msg_buffer);
        }
    }
}

/* Function to find the VGA screen specifications */
void get_screen_specs() {
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE; // pixel controller
    int resolution_reg, status_reg;

    resolution_reg = *(pixel_ctrl_ptr + 2);
    resolution_x = resolution_reg & 0xFFFF;
    resolution_y = resolution_reg >> 16;
    status_reg = *(pixel_ctrl_ptr + 3);
    sizeof_pixel = (status_reg & 0xF00) == 0x700 ? 1 : 2; // check bits 11-8 for sizeof (pixel)
    video_m = (status_reg & 0xFF000000) >> 24;            // width of y address
    video_n = (status_reg & 0x00FF0000) >> 16;            // width of x address
    shift_x = sizeof_pixel - 1;                // shift x address bits by sizeof(pixel)
    shift_y = video_n + (sizeof_pixel - 1);    // shift y address by |x address| + sizeof(pixel)
}

/* Function to blank the VGA screen */
void clear_screen() {
    int y, x;

    for (y = 0; y < resolution_y; y++)
       for (x = 0; x < resolution_x; x += 2)   // clear two pixels at a time
            *(int *)(pixel_buffer_start + (y << shift_y) + (x << shift_x)) = 0;
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

/* Function to find the text screen specifications */
void get_char_specs() {
    volatile int * char_ctrl_ptr = (int *)CHAR_BUF_CTRL_BASE; // character controller
    int resolution_reg, status_reg;

    resolution_reg = *(char_ctrl_ptr + 2);
    char_x = resolution_reg & 0xFFFF;           // global variable
    char_y = resolution_reg >> 16;              // global variable
    status_reg = *(char_ctrl_ptr + 3);
    char_m = (status_reg & 0xFF000000) >> 24;   // global for width of y address
    char_n = (status_reg & 0x00FF0000) >> 16;   // global for width of x address
}

/* Function to erase the character buffer */
void erase_chars( ) {
    int x, y;
    for (x = 0; x < char_x; x++)
        for (y = 0; y < char_y; y++)
            draw_char (x, y, ' ');
}

/* setup the A9 private timer */
void config_timer() {
    volatile int * timer_ptr = (int *)MTIME_BASE; // machine timer base address

    int counter  = TIMEOUT;    // timeout
    *(timer_ptr) = counter;    // write to mtimecmp low
    *(timer_ptr + 1) = 0;      // write to mtimecmp high

    /* reset the timer */
    *(timer_ptr + 2) = 0;      // write to mtime low
    *(timer_ptr + 3) = 0;      // write to mtime high
}

/* check if timer has expired */
int timer_expired() {
    volatile int * timer_ptr = (int *)MTIME_BASE; // machine timer base address
    int status, current, limit;

    limit = *(timer_ptr);       // read mtimecmp
    current = *(timer_ptr + 2); // read mtime
    status = 0;
    if (current >= limit){
        status = 1;
        *(timer_ptr + 2) = 0;   // restart timer
    }
    return status;
}

/* my_itoa: convert n to characters in s */
void my_itoa(int n, char s[]) {
    int i, sign;

    if ((sign = n) < 0)         /* record sign */
        n = -n;                 /* make n positive */
    i = 0;
    do {                        /* generate digits in reverse order */
        s[i++] = n % 10 + '0';  /* get next digit */
    } while ((n /= 10) > 0);    /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}  

/* reverse:  reverse string s in place */
void reverse(char s[]) {
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}  

/* Code that draws the animation objects */
void draw(void) {
    int i;

    /* Erase any boxes and lines that may have been previously drawn */
    for (i = 0; i < boxes_to_draw_last; i++) {
        // Erase box
        draw_box(x_box_last[i], y_box_last[i], x_box_last[i] + BOX_LEN, y_box_last[i] + 
            BOX_LEN, BLACK);

        // Erase line that may have been drawn
        draw_line(x_box_last[i], y_box_last[i], x_box_last[(i + 1) % boxes_to_draw_last],
            y_box_last[(i + 1) % boxes_to_draw_last], BLACK);
    }
    boxes_to_draw_last = boxes_to_draw;   // needed for erasing
    for (i = 0; i < NUM_BOXES_MAX; i++) {
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
    boxes_to_draw = boxes_to_draw_new;      // update from pushbutton()
    for (i = 0; i < boxes_to_draw; i++) {   // boxes_to_draw global changed by KEYs
        // Draw new box
        draw_box(x_box[i], y_box[i], x_box[i] + BOX_LEN, y_box[i] + BOX_LEN, color_box[i]);

        if (!no_lines)
            draw_line(x_box[i], y_box[i], x_box[(i + 1) % boxes_to_draw],
                y_box[(i + 1) % boxes_to_draw], color_box[i]);
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
    error = -(deltax / 2);
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

void plot_pixel(int x, int y, short color) {
    *(short *)(pixel_buffer_start + (y << 10) + (x << 1)) = color;
}

/* Draws one character at text coordinates (x, y) */
void draw_char(int x, int y, char c) {
    *(char *)(char_buffer_start + (y << char_n) + x) = c;
}

void draw_str(int x, int y, char *msg) {
    for (; *msg; ++x, ++msg) {
        *(char *)(char_buffer_start + (y << char_n) + x) = *msg;
    }
}

/****************************************************************************************
 * This routine checks which KEY has been pressed. It performs:
 *    KEY0: speeds up the timer
 *    KEY1: slows down the timer
 *    KEY2: decreases # of boxes, lines drawn
 *    KEY3: increases # of boxes, lines drawn
 ***************************************************************************************/
void pushbutton(void) {
    volatile int * KEY_ptr = (int *)KEY_BASE;
    int press;

    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    *(KEY_ptr + 3) = press; // Clear the interrupt

    if (press & KEY0)
        reconfig_timer(FASTER);
    else if (press & KEY1)
        reconfig_timer(SLOWER);
    else if (press & KEY2) {
        --boxes_to_draw_new;
        if (boxes_to_draw_new < 1)
            boxes_to_draw_new = 1;
    }
    else if (press & KEY3) {
        ++boxes_to_draw_new;
        if (boxes_to_draw_new > NUM_BOXES_MAX)
            boxes_to_draw_new = NUM_BOXES_MAX;
    }
    return;
}

/* setup Machine Timer */
void reconfig_timer(int action) {
    int i, counter;
    volatile int * timer_ptr = (int *)MTIME_BASE; // timer base address

    /* set the timer period */
    counter = *(timer_ptr); // read mtimecmp
    if (action == FASTER) {
        counter = counter >> 1;
        if (counter < MIN_LOAD) {
            counter = MIN_LOAD; // can't draw faster than VGA sync time
            if (shifted_dx_dy < MAX_SHIFT) { // limit the speed up
                ++shifted_dx_dy;
                for (i = 0; i < NUM_BOXES_MAX; i++) {
                    dx_box[i] = dx_box[i] << 1;
                    dy_box[i] = dy_box[i] << 1;
                }
            }
        }
    }
    else {
        if (shifted_dx_dy) {
            counter = MIN_LOAD; // can't draw faster than VGA sync time
            --shifted_dx_dy;
            for (i = 0; i < NUM_BOXES_MAX; i++) {
                dx_box[i] = dx_box[i] >> 1;
                dy_box[i] = dy_box[i] >> 1;
            }
        }
        else
            counter = counter << 1;
    }
    *(timer_ptr) = counter; // write to mtimecmp
}
