#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int sumDigits(char input[]);

int main()
{
    char input[20];
    scanf("%19s", input); 

    while(strcmp(input, "FIM") != 0)
    {     
        int valor = sumDigits(input); 
        printf("%i\n", valor);

        scanf("%19s", input);
    }

    return 0;
}

int sumDigits(char input[])
{
    int valor = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] >= '0' && input[i] <= '9')
        {
            valor += input[i] - '0';
        }
    }

    return valor;
}