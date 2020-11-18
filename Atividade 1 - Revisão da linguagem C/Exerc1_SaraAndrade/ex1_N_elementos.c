#include "stdio.h"
#include "mergeSort.h"

/* Função main para um array de N elementos */
int main(void)
{
    int N;
    printf("Digite o tamanho do array: ");
    scanf("%d", &N);

    int array[N];

    for(int i = 0; i < N; i++)
    {
        scanf("%d", &array[i]);
    }

    int array_size = sizeof(array) / sizeof(array[0]);

    printf("O array digitado: \n");
    printArray(array, array_size);

    mergeSort(array, 0, array_size - 1);

    printf("\nO array ordenado: \n");
    printArray(array, array_size);

    return 0;
}