#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main()
{
    int jogadas;
    scanf("%d", &jogadas);

    while (jogadas != 0)
    {
        int bakugansMark[jogadas];
        int poderTotalMark = 0;

        int bakugansLeti[jogadas];
        int poderTotalLeti = 0;

        int pontoExtraVerificaMark = -1;
        int pontoExtraVerificaLeti = -1;

        for (int i = 0; i < jogadas; i++)
        {
            scanf("%d", &bakugansMark[i]);
            poderTotalMark += bakugansMark[i];
        }

        for (int i = 0; i < jogadas - 2; i++)
        {
            if(bakugansMark[i] == bakugansMark[i+1] && bakugansMark[i] == bakugansMark[i+2] && pontoExtraVerificaMark == -1){
                pontoExtraVerificaMark = i;
            }
        }

        for (int i = 0; i < jogadas; i++)
        {
            scanf("%d", &bakugansLeti[i]);
            poderTotalLeti += bakugansLeti[i];
        }

        for (int i = 0; i < jogadas - 2; i++)
        {
            if(bakugansLeti[i] == bakugansLeti[i+1] && bakugansLeti[i] == bakugansLeti[i+2] && pontoExtraVerificaLeti == -1){
                pontoExtraVerificaLeti = i;
            }
        }

        if(pontoExtraVerificaMark != -1 && pontoExtraVerificaLeti == -1){
            poderTotalMark += 30;
        }
        else if(pontoExtraVerificaMark == -1 && pontoExtraVerificaLeti != -1){
            poderTotalLeti += 30;
        }
        else if(pontoExtraVerificaMark != -1 && pontoExtraVerificaLeti != -1){
            if(pontoExtraVerificaMark < pontoExtraVerificaLeti){
                poderTotalMark += 30;
            }
            else if(pontoExtraVerificaMark > pontoExtraVerificaLeti){
                poderTotalLeti += 30;
            }
        }

        if (poderTotalLeti > poderTotalMark)
        {
            printf("L\n");
        }
        else if (poderTotalLeti < poderTotalMark)
        {
            printf("M\n");
        }
        else
        {
            printf("T\n");
        }

        scanf("%d", &jogadas);
    }
}