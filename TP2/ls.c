#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isVogais(char a[1000])
{
    strlwr(a);
    int len = strlen(a);
    for (int i = 0; i < len; i++)
    {
        if (a[i] == '\n' || a[i] == '\r') continue;
        if (a[i] != 'a' && a[i] != 'e' && a[i] != 'i' && a[i] != 'o' && a[i] != 'u' && a[i] != ' ')
        {
            return false;
        }
    }
    return true;
}

bool isConsoantes(char b[1000])
{
    strlwr(b);
    int len = strlen(b);
    for (int i = 0; i < len; i++)
    {
        if (b[i] == '\n' || b[i] == '\r') continue;
        if ((b[i] < 'a' || b[i] > 'z') && b[i] != ' ')
        {
            return false;
        }
        if (b[i] == 'a' || b[i] == 'e' || b[i] == 'i' || b[i] == 'o' || b[i] == 'u')
        {
            return false;
        }
    }
    return true;
}

bool isInteiro(char c[1000])
{
    int len = strlen(c);
    int countDot = 0;
    for (int i = 0; i < len; i++)
    {
        if (c[i] > '9' || c[i] < '0')
        {
            return false;
        }
        else if (c[i] == '.' || c[i] == ',')
        {
            countDot++;
        }
        if (countDot >= 1)
        {
            return false;
        }
        
    }
    return true;
}

bool isReal(char d[1000])
{
    int len = strlen(d);
    int countDot = 0;
    for (int i = 0; i < len; i++)
    {
        if ((d[i] < '0' || d[i] > '9') && d[i] != '.' && d[i] != ',')
        {
            return false;
        }
        if (d[i] == '.' || d[i] == ',')
        {
            countDot++;
        }
        if (countDot > 1)
        {
            return false;
        }
        
    }
    return true;
}

int main()
{

    char input [1000];
    char input2 [1000];
    char input3 [1000];
    char input4 [1000];

    scanf(" %1000[^\n]", input);
    scanf(" %1000[^\n]", input2);
    scanf(" %1000[^\n]", input3);
    scanf(" %1000[^\n]", input4);

    do
    {
        isVogais(input) ? printf("SIM ") : printf("NAO ");
        isConsoantes(input2) ? printf("SIM ") : printf("NAO ");
        isInteiro(input3) ? printf("SIM ") : printf("NAO ");
        isReal(input4) ? printf("SIM\n") : printf("NAO\n");

        scanf(" %1000[^\n]", input);
        scanf(" %1000[^\n]", input2);
        scanf(" %1000[^\n]", input3);
        scanf(" %1000[^\n]", input4);
    }
    while(strcmp(input, "FIM" ) || strcmp(input2, "FIM") || strcmp(input3, "FIM") || strcmp(input4, "FIM"));
    return 0;
}