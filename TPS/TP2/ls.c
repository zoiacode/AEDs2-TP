#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool stop(char input[])
{
    return (strlen(input) == 3 && input[0] == 'F' && input[1] == 'I' && input[2] == 'M');
}

bool isVogais(char a[1000])
{
    char vogais[] = "aeiouAEIOU";
    int n = strlen(a);
    bool isVogal = false;
    for (int i = 0; i < n; i++)
    {
        isVogal = false;
        for (int j = 0; j < 10; j++)
        {
            if (a[i] == vogais[j])
            {
                isVogal = true;
                break;
            }
        }
        if(!isVogal)
        {
            return false;
        }
    }
    return isVogal;
}

bool isConsoantes(char b[1000])
{
    char vogais[] = "aeiouAEIOU";
    int n = strlen(b);
    bool isConsoante = true;
    for (int i = 0; i < n; i++)
    {
        isConsoante = true;
        if (!((b[i] >= 'a' && b[i] <= 'z') || (b[i] >= 'A' && b[i] <= 'Z')))
        {
           return false; 
        }
        for (int j = 0; j < 10; j++)
        {
            if (b[i] == vogais[j])
            {
                return false;
            }
        }
    }
    return isConsoante;
}

bool isInteiro(char c[1000])
{
    char numeros[] = "0123456789";
    int n = strlen(c);
    bool isNumero = false;
    for (int i = 0; i < n; i++)
    {
        isNumero = false;
        for (int j = 0; j < 10; j++)
        {
            if (c[i] == numeros[j])
            {
                isNumero = true;
                break;
            }
        }
        if(!isNumero)
        {
            return false;
        }
    }
    return isNumero;
}

bool isReal(char d[1000])
{
    char numeros[] = "0123456789.,";
    int n = strlen(d);
    bool isNumeroReal = false;
    int contadorPonto = 0;
    for (int i = 0; i < n; i++)
    {
        isNumeroReal = false;
        if(d[i] == '.' || d[i] == ',')
        {
            contadorPonto++;
        }
        
        for (int j = 0; j < 12; j++)
        {
            if (d[i] == numeros[j])
            {
                isNumeroReal = true;
                break;
            }
        }
        if(!isNumeroReal)
        {
            return false;
        }
    }
    if (contadorPonto > 1)
    {
        isNumeroReal = false;
    }

    return isNumeroReal;
}

void removerNovaLinha(char s[])
{
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
    {
        s[len - 1] = '\0';
    }
}

int main()
{
    char firstInput[1000];
    fgets(firstInput, sizeof(firstInput), stdin);
    removerNovaLinha(firstInput);

    while (!stop(firstInput))
    {
        printf("%s ", isVogais(firstInput) ? "SIM" : "NAO");
        printf("%s ", isConsoantes(firstInput) ? "SIM" : "NAO");
        printf("%s ", isInteiro(firstInput) ? "SIM" : "NAO");
        printf("%s\n", isReal(firstInput) ? "SIM" : "NAO");

        fgets(firstInput, sizeof(firstInput), stdin);
        removerNovaLinha(firstInput);
    }
}