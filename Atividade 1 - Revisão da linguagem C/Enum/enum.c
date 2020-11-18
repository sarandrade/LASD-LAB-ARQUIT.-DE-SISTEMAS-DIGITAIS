#include "stdio.h"

enum Cores
{
    amarelo = 0,
    vermelho = 1,
    azul = 2
};

int main(void)
{
    enum Cores minhaCor;
    minhaCor = vermelho;
    printf("%d", minhaCor);
    return 0;
}