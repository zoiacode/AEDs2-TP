#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    char nome[100];
    float preco;
    int prioridade;
} Presente;

void bubbleSort(Presente presente[], int qntdPresentes);
void printPresente(Presente presente[], char nomePessoa[], int qntdPresentes);

int main()
{
    char linha [100];
    while (fgets(linha, sizeof(linha), stdin) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';
        char nomePessoa[100];
        int qntdPresentes = 0;

        sscanf(linha, "%s %d", nomePessoa, &qntdPresentes);

        Presente presente[qntdPresentes];

        for (int i = 0; i < qntdPresentes; i++)
        {
            fgets(presente[i].nome, sizeof(presente[i].nome), stdin);
            presente[i].nome[strcspn(presente[i].nome, "\n")] = '\0';

            fgets(linha, sizeof(linha), stdin);
            linha[strcspn(linha, "\n")] = '\0';

            sscanf(linha, "%f %d", &presente[i].preco, &presente[i].prioridade);
        }

        bubbleSort(presente, qntdPresentes);

        printPresente(presente, nomePessoa, qntdPresentes);
    } 
    return 0;
}

void bubbleSort(Presente presente[], int qntdPresentes) {
    for (int i = 0; i < qntdPresentes - 1; i++)
    {
        for (int j = 0; j < qntdPresentes - i - 1; j++)
        {
            if(presente[j].prioridade < presente[j + 1].prioridade) {
                Presente aux = presente[j];
                presente[j] = presente[j + 1];
                presente[j + 1] = aux;
            }
            else if (presente[j].prioridade == presente[j+1].prioridade) {
                if (presente[j].preco > presente[j + 1].preco) {
                    Presente aux = presente[j];
                    presente[j] = presente[j + 1];
                    presente[j + 1] = aux;
                }else if(presente[j].preco == presente[j + 1].preco) {
                    if (strcmp(presente[j].nome, presente[j + 1].nome) > 0) {
                        Presente aux = presente[j];
                        presente[j] = presente[j + 1];
                        presente[j + 1] = aux;
                    }
                }
            }
            
        }
    }
}

void printPresente(Presente presente[], char nomePessoa[], int qntdPresentes) {
    printf("Lista de %s \n", nomePessoa);
    for (int i = 0; i < qntdPresentes; i++)
    {
        printf("%s - %.2f \n", presente[i].nome, presente[i].preco);
    }
}