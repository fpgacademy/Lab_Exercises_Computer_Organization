int SWAP(int *);

int LIST[] = {10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void)
{
    int i, flag, length, *item_ptr;

    length = LIST[0]; // number of elements to be sorted 
    do
    {
        flag     = 0;        // indicates if nothing remains to be sorted
        item_ptr = LIST + 1; // pointer to the first element of data
        for (i = 1; i < length; i++)
        {
            flag |= SWAP(item_ptr);
            ++item_ptr; // point to the next word of data
        }
        --length; // last item in the list is in the right place
    } while (flag);
}

int SWAP(int * p)
{
    int i1, i2;

    i1 = *p;
    i2 = *(p + 1);

    if (i1 < i2)
    {
        *p       = i2;
        *(p + 1) = i1;
        return (1);
    }
    else
        return (0);
}
