#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int N;
    scanf("%d", &N);

    char pokemons[1000][1001];
    int count = 0;

    for (int i = 0; i < N; i++) {
        char nome[1001];
        scanf("%s", nome);

        int existe = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(nome, pokemons[j]) == 0) {
                existe = 1;
                break;
            }
        }

        if (!existe) {
            strcpy(pokemons[count], nome);
            count++;
        }
    }

    int faltam = 151 - count;

    printf("Falta(m) %d pokemon(s)\n", faltam);

    return 0;
}
