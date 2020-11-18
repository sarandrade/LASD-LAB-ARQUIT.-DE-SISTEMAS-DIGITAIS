#include "stdio.h"
#include <conio.h>
#include <stdlib.h>
#include "mergeSort.h"

/* Função main para um array com 10000 elementos aleatórios entre 0 e 100 */
int main(void)
{
    int N = 10000;
    int array[N];

    for (int i = 0; i < N; i++)
    {
        /* Gerando valores aleatórios entre 0 e 100 e atribuindo ao array[] */
        array[i] = rand() % 100;
    }

    int array_size = sizeof(array) / sizeof(array[0]);

    printf("O array sorteado: \n");
    printArray(array, array_size);

    mergeSort(array, 0, array_size - 1);

    printf("\nO array ordenado: \n");
    printArray(array, array_size);

    return 0;
}