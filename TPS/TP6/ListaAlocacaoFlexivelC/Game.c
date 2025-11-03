#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE 8192
#define MAX_FIELD 2048
#define MAX_ITEM_LEN 256
#define MAX_GAMES 50000

typedef struct {
    int id;
    char name[512];
    char releaseDate[16];
    int estimatedOwners;
    float price;
    // Outros campos omitidos para brevidade, você pode adicionar se quiser
} Game;

typedef struct Node {
    Game* game;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int size;
} LinkedList;

// Funções da lista encadeada
void initList(LinkedList* list) {
    list->head = NULL;
    list->size = 0;
}

void inserirInicio(LinkedList* list, Game* game) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->game = game;
    newNode->next = list->head;
    list->head = newNode;
    list->size++;
}

void inserirFim(LinkedList* list, Game* game) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->game = game;
    newNode->next = NULL;
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node* cur = list->head;
        while (cur->next != NULL) cur = cur->next;
        cur->next = newNode;
    }
    list->size++;
}

void inserir(LinkedList* list, Game* game, int pos) {
    if (pos < 0 || pos > list->size) {
        fprintf(stderr, "Posição inválida para inserção: %d\n", pos);
        return;
    }
    if (pos == 0) {
        inserirInicio(list, game);
        return;
    }
    if (pos == list->size) {
        inserirFim(list, game);
        return;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->game = game;
    Node* cur = list->head;
    for (int i = 0; i < pos - 1; i++) {
        cur = cur->next;
    }
    newNode->next = cur->next;
    cur->next = newNode;
    list->size++;
}

Game* removerInicio(LinkedList* list) {
    if (list->head == NULL) return NULL;
    Node* toRemove = list->head;
    Game* game = toRemove->game;
    list->head = toRemove->next;
    free(toRemove);
    list->size--;
    return game;
}

Game* removerFim(LinkedList* list) {
    if (list->head == NULL) return NULL;
    if (list->head->next == NULL) {
        Game* game = list->head->game;
        free(list->head);
        list->head = NULL;
        list->size--;
        return game;
    }
    Node* cur = list->head;
    while (cur->next->next != NULL) cur = cur->next;
    Game* game = cur->next->game;
    free(cur->next);
    cur->next = NULL;
    list->size--;
    return game;
}

Game* remover(LinkedList* list, int pos) {
    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "Posição inválida para remoção: %d\n", pos);
        return NULL;
    }
    if (pos == 0) return removerInicio(list);
    if (pos == list->size - 1) return removerFim(list);
    Node* cur = list->head;
    for (int i = 0; i < pos - 1; i++) {
        cur = cur->next;
    }
    Node* toRemove = cur->next;
    Game* game = toRemove->game;
    cur->next = toRemove->next;
    free(toRemove);
    list->size--;
    return game;
}

// Função para buscar Game* pelo id no array allGames
Game* buscarGamePorId(Game* allGames, int total, int id) {
    for (int i = 0; i < total; i++) {
        if (allGames[i].id == id) return &allGames[i];
    }
    return NULL;
}

// Função para ler CSV simplificada (apenas id e nome para exemplo)
int lerCSV(Game* allGames) {
    FILE* file = fopen("games.csv", "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir games.csv\n");
        return 0;
    }
    char line[MAX_LINE];
    fgets(line, MAX_LINE, file); // pular cabeçalho
    int count = 0;
    while (fgets(line, MAX_LINE, file)) {
        if (count >= MAX_GAMES) break;
        char* token = strtok(line, ",");
        if (!token) continue;
        allGames[count].id = atoi(token);
        token = strtok(NULL, ",");
        if (!token) continue;
        // Remove aspas e copia nome
        char* nome = token;
        if (nome[0] == '"') nome++;
        int len = strlen(nome);
        if (nome[len-1] == '\n') nome[len-1] = '\0';
        if (nome[len-2] == '"') nome[len-2] = '\0';
        strncpy(allGames[count].name, nome, 511);
        allGames[count].name[511] = '\0';
        count++;
    }
    fclose(file);
    return count;
}

int main() {
    Game allGames[MAX_GAMES];
    int totalGames = lerCSV(allGames);

    LinkedList lista;
    initList(&lista);

    // Primeira parte: ler IDs para inserir no fim da lista
    char input[256];
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "FIM") == 0) break;
        int id = atoi(input);
        Game* g = buscarGamePorId(allGames, totalGames, id);
        if (g) {
            inserirFim(&lista, g);
        }
    }

    // Segunda parte: ler número de comandos
    int n;
    scanf("%d\n", &n);

    for (int i = 0; i < n; i++) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strncmp(input, "II ", 3) == 0) {
            // Inserir no início
            char filename[256];
            sscanf(input + 3, "%s", filename);
            // Ler jogo do arquivo
            // Para simplificar, vamos assumir que filename é o id do jogo
            int id = atoi(filename);
            Game* g = buscarGamePorId(allGames, totalGames, id);
            if (g) inserirInicio(&lista, g);
        } else if (strncmp(input, "IF ", 3) == 0) {
            // Inserir no fim
            char filename[256];
            sscanf(input + 3, "%s", filename);
            int id = atoi(filename);
            Game* g = buscarGamePorId(allGames, totalGames, id);
            if (g) inserirFim(&lista, g);
        } else if (strncmp(input, "I*", 2) == 0) {
            // Inserir em posição
            int pos, id;
            sscanf(input + 2, "%d %d", &pos, &id);
            Game* g = buscarGamePorId(allGames, totalGames, id);
            if (g) inserir(&lista, g, pos);
        } else if (strcmp(input, "RI") == 0) {
            // Remover do início
            Game* g = removerInicio(&lista);
            if (g) printf("(R) %s\n", g->name);
        } else if (strncmp(input, "R*", 2) == 0) {
            // Remover de posição
            int pos;
            sscanf(input + 2, "%d", &pos);
            Game* g = remover(&lista, pos);
            if (g) printf("(R) %s\n", g->name);
        } else if (strcmp(input, "RF") == 0) {
            // Remover do fim
            Game* g = removerFim(&lista);
            if (g) printf("(R) %s\n", g->name);
        }
    }

    // Imprimir todos os jogos restantes na lista
    Node* cur = lista.head;
    while (cur) {
        printf("%s\n", cur->game->name);
        cur = cur->next;
    }

    // Liberar memória da lista (não libera jogos, pois são do array allGames)
    while (lista.head) {
        Node* tmp = lista.head;
        lista.head = lista.head->next;
        free(tmp);
    }

    return 0;
}