#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <stdbool.h>

bool isVogal(char input[], int pos);
bool isConsoante(char input[], int pos);
bool isInteiro(char input[], int pos);
bool isReal(char input[], int pos, int contador);

int main()
{
    char input[100];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    
    while (strcmp(input, "FIM") != 0)
    {
        if (strcmp(input, "FIM") != 0)
        {
            printf("%s ", isVogal(input, 0) ? "SIM" : "NAO");
            printf("%s ", isConsoante(input, 0) ? "SIM" : "NAO");
            printf("%s ", isInteiro(input, 0) ? "SIM" : "NAO");
            printf("%s\n", isReal(input, 0, 0) ? "SIM" : "NAO");
        }

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
    }
}

bool isVogal(char input[], int pos) {
    char vogais[] = "aeiouAEIOU";
    if (input[pos] == '\0')
    {
        return true;
    }
    for (int j = 0; j < strlen(vogais); j++) {
        if (input[pos] == vogais[j])
        {
            return isVogal(input, pos + 1);
        }
            
    }
    return false;
}

bool isConsoante(char input[], int pos) {
    char vogais[] = "aeiouAEIOU";

    if (input[pos] == '\0')
    {
        return true;
    }

    char ch = input[pos];

    if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
    {
        return false; 
    }

    for (int j = 0; j < strlen(vogais); j++) {
        if (ch == vogais[j])
        {
            return false;
        }
    }

    return isConsoante(input, pos + 1);
}


bool isInteiro(char input[], int pos) {
    if (input[pos] == '\0')
    {
        return true;
    }

    if (input[pos] < '0' || input[pos] > '9')
    {
        return false;
    }

    return isInteiro(input, pos + 1);
}

bool isReal(char input[], int pos, int contador) {
    if (input[pos] == '\0')
    {
        return true;
    }

    char ch = input[pos];

    if ((ch < '0' || ch > '9') && ch != '.' && ch != ',')
    {
        return false;
    }

    if (ch == '.' || ch == ',') {
        contador++;
        if (contador > 1)
        {
            return false;
        }
    }

    return isReal(input, pos + 1, contador);
}
