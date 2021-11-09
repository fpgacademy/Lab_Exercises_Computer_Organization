int FINDSUM(int N)
{
    int sum = 0;
    while (N != 0)
    {
        sum = sum + N;
        N   = N - 1;
    }
    return sum;
}
