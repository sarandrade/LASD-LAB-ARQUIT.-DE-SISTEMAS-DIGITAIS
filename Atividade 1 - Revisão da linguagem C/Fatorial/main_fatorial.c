#include <stdio.h>
#include "fatorial.h"

int main(void)
{
    for (int j = 1; j <= 5; j++)
    {
        int n_fat = fatorial(j);
        printf("!%d = %d\n", j, n_fat);
    }

    while (1)
    {
        int N;
        scanf("%d", &N);
        printf("!%d = %d\n", N, fatorial(N));
    }

    return 0;
}