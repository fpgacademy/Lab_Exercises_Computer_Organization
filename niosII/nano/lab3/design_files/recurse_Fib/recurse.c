#include <stdio.h>
/* Calculates a desired Fibonnaci number. The first few Fibonnaci numbers are:
 * 0 1 1 2 3 5 8 13 21 34 55 ...
 */

int FIBONNACI(int);

int main(void)
{
    int Fib;

    Fib = FIBONNACI(10);
    printf("Fibonnaci(10) = %d\n", Fib);
}

int FIBONNACI(int N)
{
    if (N < 2)
        return N;
    else
        return FIBONNACI(N - 1) + FIBONNACI(N - 2);
}

