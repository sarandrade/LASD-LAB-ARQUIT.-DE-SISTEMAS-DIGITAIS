// Struct

#include "stdio.h"
#include <string.h>

/* struct Livro
{
    char titulo[50];
    char autor[50];
    int id;
} livroX;

int main(void)
{
    struct Livro livroY;

    strcpy(livroY.titulo, "A Origem");
    strcpy(livroX.titulo, "Pantera Negra");

    printf("%s\n", livroX.titulo);
    printf("%s", livroY.titulo);

    return 0;
} */

typedef struct Livro
{
    char titulo[50];
    char autor[50];
    int id;
} Livro;

int main(void)
{
    Livro livroX, livroY;

    strcpy(livroY.titulo, "A Origem");
    strcpy(livroX.titulo, "Pantera Negra");

    printf("%s\n", livroX.titulo);
    printf("%s", livroY.titulo);

    return 0;
}