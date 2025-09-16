#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void Contador(int contador);

int main()
{
    char letras[6];
    int contador = 0;

    while (1)
    {
        contador = 0;
        for (int i = 0; i < 6; i++)
        {
            if (scanf(" %c", &letras[i]) != 1)
            {
                return 0;
            }
            if (letras[i] == 'V')
            {
                contador++;
            }
        }
        Contador(contador);
    }
}

void Contador(int contador)
{
    if (contador == 5 || contador == 6)
    {
        printf("1\n");
    }else if (contador == 3 || contador == 4)
    {
        printf("2\n");
    }else if (contador == 1 || contador == 2)
    {
        printf("3\n");
    }else
    {
        printf("-1\n");
    }
}
