#include "fatorial.h"

int fatorial(int N)
{
    int n_fat = 1;
    for (int i = 1; i <= N; i++)
    {
        n_fat *= i;
    }
    return n_fat;
}