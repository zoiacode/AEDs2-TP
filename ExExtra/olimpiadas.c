#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

 
struct Paises
{
    char nome[30];
    int ouro;
    int prata;
    int bronze;
};

int main ()
{
    int qntdPaises;
    scanf("%d", &qntdPaises);
    struct Paises paises[qntdPaises];

    for (int i = 0; i < qntdPaises; i++)
    {
        scanf("%s %d %d %d", paises[i].nome, &paises[i].ouro, &paises[i].prata, &paises[i].bronze);
    }

    //insertion sort
    for(int i = 1; i < qntdPaises; i++)
    {
        int j = i - 1;
        struct Paises aux = paises[i];
        while (j >= 0 && (paises[j].ouro < aux.ouro || (paises[j].ouro == aux.ouro && paises[j].prata < aux.prata) || (paises[j].ouro == aux.ouro && paises[j].prata == aux.prata && paises[j].bronze < aux.bronze)))
        {
            paises[j + 1] = paises[j];
            j--;
        }
        paises[j + 1] = aux;
    }
    
    for (int i = 0; i < qntdPaises; i++)
    {
        printf("%s %d %d %d\n", paises[i].nome, paises[i].ouro, paises[i].prata, paises[i].bronze);
    }
}