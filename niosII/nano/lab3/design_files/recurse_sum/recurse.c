#include <stdio.h>
/* calculates sum of numbers from 1 to N, using recursion */

int FINDSUM(int);

int main(void)
{
    int Sum;

    Sum = FINDSUM(5);
    printf("N = 5, Sum = %d\n", Sum);
}

int FINDSUM(int N)
{
    if (N == 0)
        return 0;
    else
        return N + FINDSUM(N - 1);
}

