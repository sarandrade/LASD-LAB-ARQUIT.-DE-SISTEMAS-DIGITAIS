#include <stdio.h>
#include <stdlib.h>
#include "mergeSort.h"

/* O algorimmo de ordenação utilizado é o Marge Sort */
/*  l representa o índice da esquerda (left)
    r representa o índice da direita (right) 
    do sub-array do array a ser ordenado */
void mergeSort(int array[], int l, int r)
{
    if(l < r)
    {
        int m = l + (r - l) / 2;

        // Ordena a primeira e a segunda metade do array
        mergeSort(array, l, m);
        mergeSort(array, m + 1, r);

        merge(array, l, m, r);
    }
}

/* Mescla os dois sub-arrays do array[]
O Primeiro sub-array é array[l..m]
O segundo sub-array é arr[m+1..r] */
void merge(int array[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* Criando os arrays temporários L[] e R[] */
    int L[n1], R[n2];

    /* Copiando os dados para os arrays temporários L[] e R[] */
    for (i = 0; i < n1; i++)
    {
        L[i] = array[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = array[m + 1 + j];
    }

    /* Mescando os arrays temporários novamento no array[l...r] */
    i = 0; // Índice inicial do primeiro sub-array
    j = 0; // Índice inicial do segundo sub-array
    k = l; // Índice inicial do sub-array fundido

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            array[k] = L[i];
            i++;
        }
        else
        {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copiando os elementos restantes de L[], se tiver algum */
    while (i < n1)
    {
        array[k] = L[i];
        i++;
        k++;
    }

    /* Copiando os elementos restantes de R[], se tiver algum */
    while (j < n2)
    {
        array[k] = R[j];
        j++;
        k++;
    }
}

/* Função para exibir o array */
void printArray(int array[], int N)
{
    printf("[");
    for(int j = 0; j < N; j++)
    {
        if(j == N - 1)
        {
            printf("%d", array[j]);
            break;
        }
        printf("%d, ", array[j]);
    }
    printf("]");
}