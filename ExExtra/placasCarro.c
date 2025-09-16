#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    char placa[100];

    
    if(strlen(placa) != 8 && strlen(placa) != 7)
    {
        printf("0\n");
        return 0;
    }

    if(placa[3] != '-')
    {
        for (int i = 0; i < 3; i++)
        {
            if (placa[i] < 'A' || placa[i] > 'Z')
            {
                printf("0\n");
                return 0;
            }
        }

        if(placa[3] < '0' || placa[3] > '9')
        {
            printf("0\n");
            return 0;
        }
        if (placa[4] < 'A' || placa[4] > 'Z')
        {
            printf("0\n");
            return 0;
        }
        if (placa[5] < '0' || placa[5] > '9')
        {
            printf("0\n");
            return 0;
        }
        if (placa[6] < '0' || placa[6] > '9')
        {
            printf("0\n");
            return 0;
        }

        printf("2\n");
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            if (placa[i] < 'A' || placa[i] > 'Z')
            {
                printf("0\n");
                return 0;
            }
        }

        if (placa[3] != '-')
        {
            printf("0\n");
            return 0;
        }
        
        for (int i = 4; i < 8; i++)
        {
            if (placa[i] < '0' || placa[i] > '9')
            {
                printf("0\n");
                return 0;
            }
        }

        printf("1\n");
    }
    
}