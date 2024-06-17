// 7-segment display bit code for digits 0 to 9
char seg7[10] =  {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
                  0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};

int main(void) 
{
    int MM, SS, DD;
    // declare other variables, pointers (code not shown)
    .
    .
    // initialize Nios V Machine Timer (code not shown)
    .
    .
    time = 0;   // start time at 00:00:00

    while (1) {
        // read the KEY pushbuttons (code not shown)
        .
        .
        // extract minutes, seconds, hundredths from time (code not shown)
        MM = ...
        SS = ...
        DD = ...
        // display on HEX5-0
        *HEX5_HEX4_ptr = (seg7[MM / 10] << 8) | seg7[MM % 10];
        *HEX3_HEX0_ptr = ...

        // increment time (code not shown)
        .
        .
        // wait for timer, then restart timer (code not shown)
        .
        .
    }
}
