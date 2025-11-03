#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAX_LINE 8192
#define MAX_FIELD 2048
#define MAX_LIST_ITEMS 100
#define MAX_ITEM_LEN 256

typedef struct {
    char** items;
    int count;
} StringList;

typedef struct {
    int id;
    char name[512];
    char releaseDate[16];
    int estimatedOwners;
    float price;
    StringList supportedLanguages;
    int metacriticScore;
    float userScore;
    int achievements;
    StringList publishers;
    StringList developers;
    StringList categories;
    StringList genres;
    StringList tags;
} Game;

long long comparacoes = 0;
long long movimentacoes = 0;

StringList createStringList();
void addToStringList(StringList* list, const char* item);
void freeStringList(StringList* list);
void freeStringList_actual(StringList* list); 

void cleanQuotes(char* s);
int parseIntOrDefault(char* s, int def);
float parseFloatOrDefault(char* s, float def);
int parseOwners(char* s);
float parsePrice(char* s);
float parseUserScore(char* s);
void normalizeDate(char* s, char* output);
void trim(char* s);
void parseListBracketed(char* s, StringList* list);
void parseListComma(char* s, StringList* list);
void formatList(StringList* list, char* output);
void formatFloat(float v, char* output);
void formatPrice(float v, char* output);
void formatUserScore(float v, char* output);
void toJudgeLine(Game* g, char* output);
void splitCSV(char* line, char fields[][MAX_FIELD], int* fieldCount);

int compareGames(Game* g1, Game* g2);
void swap(Game* a, int i, int j);
int partition(Game* array, int low, int high);
void quicksortRec(Game* array, int low, int high);
void quicksort(Game* array, int n);

typedef struct No {
    Game* elemento;
    struct No* prox;
} No;

No* newNo(Game* game, No* prox) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        perror("Erro ao alocar No");
        exit(EXIT_FAILURE);
    }
    novo->elemento = game;
    novo->prox = prox;
    return novo;
}

typedef struct ListaGames {
    No* primeiro;
    No* ultimo;
    int tamanho;
} ListaGames;

ListaGames* newListaGames() {
    ListaGames* lista = (ListaGames*)malloc(sizeof(ListaGames));
    if (lista == NULL) {
        perror("Erro ao alocar ListaGames");
        exit(EXIT_FAILURE);
    }
    lista->primeiro = newNo(NULL, NULL); 
    lista->ultimo = lista->primeiro;
    lista->tamanho = 0;
    return lista;
}

No* getNo(ListaGames* lista, int pos) {
    No* i = lista->primeiro;
    for (int j = 0; j < pos && i->prox != NULL; j++, i = i->prox);
    return i;
}

void inserirInicio(ListaGames* lista, Game* game) {
    No* novo = newNo(game, lista->primeiro->prox);
    lista->primeiro->prox = novo;
    if (lista->primeiro == lista->ultimo) {
        lista->ultimo = novo;
    }
    lista->tamanho++;
}

void inserirFim(ListaGames* lista, Game* game) {
    lista->ultimo->prox = newNo(game, NULL);
    lista->ultimo = lista->ultimo->prox;
    lista->tamanho++;
}

void inserir(ListaGames* lista, Game* game, int pos) {
    if (pos < 0 || pos > lista->tamanho) {
        fprintf(stderr, "Erro ao inserir: Posicao invalida!\n");
        return;
    }
    if (pos == 0) {
        inserirInicio(lista, game);
    } else if (pos == lista->tamanho) {
        inserirFim(lista, game);
    } else {
        No* ant = getNo(lista, pos);
        No* novo = newNo(game, ant->prox);
        ant->prox = novo;
        lista->tamanho++;
    }
}

Game* removerInicio(ListaGames* lista) {
    if (lista->primeiro == lista->ultimo) {
        fprintf(stderr, "Erro ao remover: Lista vazia!\n");
        return NULL;
    }
    No* temp = lista->primeiro->prox;
    lista->primeiro->prox = temp->prox;
    if (temp == lista->ultimo) {
        lista->ultimo = lista->primeiro;
    }
    Game* resp = temp->elemento;
    free(temp);
    lista->tamanho--;
    return resp;
}

Game* removerFim(ListaGames* lista) {
    if (lista->primeiro == lista->ultimo) {
        fprintf(stderr, "Erro ao remover: Lista vazia!\n");
        return NULL;
    }
    No* i;
    for (i = lista->primeiro; i->prox != lista->ultimo; i = i->prox);
    Game* resp = lista->ultimo->elemento;
    free(lista->ultimo);
    lista->ultimo = i;
    lista->ultimo->prox = NULL;
    lista->tamanho--;
    return resp;
}

Game* remover(ListaGames* lista, int pos) {
    if (lista->primeiro == lista->ultimo || pos < 0 || pos >= lista->tamanho) {
        fprintf(stderr, "Erro ao remover: Posicao invalida ou lista vazia!\n");
        return NULL;
    }
    if (pos == 0) {
        return removerInicio(lista);
    } else if (pos == lista->tamanho - 1) {
        return removerFim(lista);
    } else {
        No* ant = getNo(lista, pos);
        No* temp = ant->prox;
        ant->prox = temp->prox;
        Game* resp = temp->elemento;
        free(temp);
        lista->tamanho--;
        return resp;
    }
}

void mostrarListaAsJudgeLines(ListaGames* lista) {
    char output[MAX_LINE * 2];
    for (No* i = lista->primeiro->prox; i != NULL; i = i->prox) {
        toJudgeLine(i->elemento, output);
        printf("%s\n", output);
    }
}

StringList createStringList() {
    StringList list;
    list.items = (char**)malloc(MAX_LIST_ITEMS * sizeof(char*));
    list.count = 0;
    return list;
}

void addToStringList(StringList* list, const char* item) {
    if (list->count < MAX_LIST_ITEMS) {
        list->items[list->count] = (char*)malloc(MAX_ITEM_LEN);
        strncpy(list->items[list->count], item, MAX_ITEM_LEN - 1);
        list->items[list->count][MAX_ITEM_LEN - 1] = '\0';
        list->count++;
    }
}

void freeStringList_actual(StringList* list) {
    if (!list) return;
    for (int i = 0; i < list->count; i++) {
        free(list->items[i]);
    }
    free(list->items);
}

void freeStringList(StringList* list) { (void)list; }


void cleanQuotes(char* s) {
    if (s == NULL) return;

    while (*s && isspace((unsigned char)*s)) s++;

    int len = strlen(s);
    if (len == 0) return;

    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }

    if (len >= 2 && ((s[0] == '"' && s[len-1] == '"') || (s[0] == '\'' && s[len-1] == '\''))) {
        memmove(s, s + 1, len - 2);
        s[len - 2] = '\0';
    }
}

int parseIntOrDefault(char* s, int def) {
    cleanQuotes(s);
    if (strlen(s) == 0) return def;

    char* endptr;
    long val = strtol(s, &endptr, 10);
    if (endptr == s || *endptr != '\0') return def;
    return (int)val;
}

float parseFloatOrDefault(char* s, float def) {
    cleanQuotes(s);
    if (strlen(s) == 0) return def;

    char* endptr;
    float val = strtof(s, &endptr);
    if (endptr == s) return def;
    return val;
}

int parseOwners(char* s) {
    cleanQuotes(s);
    if (strlen(s) == 0) return 0;

    char digits[256] = "";
    int idx = 0;
    for (int i = 0; s[i] && idx < 255; i++) {
        if (isdigit((unsigned char)s[i])) {
            digits[idx++] = s[i];
        }
    }
    digits[idx] = '\0';

    if (strlen(digits) == 0) return 0;
    return atoi(digits);
}

float parsePrice(char* s) {
    cleanQuotes(s);
    if (strlen(s) == 0) return 0.0f;
    if (strcasecmp(s, "Free to Play") == 0) return 0.0f;

    char cleaned[256] = "";
    int idx = 0;
    for (int i = 0; s[i] && idx < 255; i++) {
        if (s[i] != '$' && s[i] != ',') {
            cleaned[idx++] = s[i];
        }
    }
    cleaned[idx] = '\0';

    return parseFloatOrDefault(cleaned, 0.0f);
}

float parseUserScore(char* s) {
    cleanQuotes(s);
    if (strlen(s) == 0) return -1.0f;
    if (strcasecmp(s, "tbd") == 0) return -1.0f;
    return parseFloatOrDefault(s, -1.0f);
}

void normalizeDate(char* s, char* output) {
    cleanQuotes(s);
    if (strlen(s) == 0) {
        strcpy(output, "01/01/0001");
        return;
    }

    if (strlen(s) == 4 && isdigit((unsigned char)s[0]) && isdigit((unsigned char)s[1]) && isdigit((unsigned char)s[2]) && isdigit((unsigned char)s[3])) {
        sprintf(output, "01/01/%s", s);
        return;
    }

    struct tm tm_date = {0};
    char* formats[] = {
        "%b %d, %Y",
        "%B %d, %Y",
        "%d %b, %Y",
        "%d %B, %Y",
        "%b %Y",
        "%B %Y",
        NULL
    };

    for (int i = 0; formats[i] != NULL; i++) {
        if (strptime(s, formats[i], &tm_date) != NULL) {
            sprintf(output, "%02d/%02d/%04d", tm_date.tm_mday, tm_date.tm_mon + 1, tm_date.tm_year + 1900);
            return;
        }
    }

    char digits[256] = "";
    int idx = 0;
    for (int i = 0; s[i]; i++) {
        if (isdigit((unsigned char)s[i])) {
            digits[idx++] = s[i];
            digits[idx] = '\0';
        } else if (idx > 0 && digits[idx-1] != ' ') {
            digits[idx++] = ' ';
            digits[idx] = '\0';
        }
    }

    int nums[3] = {0};
    int count = 0;
    char* token = strtok(digits, " ");
    while (token && count < 3) {
        nums[count++] = atoi(token);
        token = strtok(NULL, " ");
    }

    if (count == 3) {
        if (nums[0] > 12) {
            sprintf(output, "%02d/%02d/%04d", nums[0], nums[1], nums[2]);
        } else if (nums[1] > 12) {
            sprintf(output, "%02d/%02d/%04d", nums[1], nums[0], nums[2]);
        } else {
            sprintf(output, "%02d/%02d/%04d", nums[1], nums[0], nums[2]);
        }
    } else if (count == 2) {
        sprintf(output, "01/%02d/%04d", nums[0], nums[1]);
    } else {
        strcpy(output, "01/01/0001");
    }
}

void trim(char* s) {
    if (s == NULL) return;

    char* start = s;
    while (*start && isspace((unsigned char)*start)) start++;

    char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;

    int len = end - start + 1;
    memmove(s, start, len);
    s[len] = '\0';
}

void parseListBracketed(char* s, StringList* list) {
    if (s == NULL || strlen(s) == 0) return;

    char* start = strchr(s, '[');
    char* end = strrchr(s, ']');

    char inner[MAX_FIELD];
    if (start && end && end > start) {
        int len = end - start - 1;
        strncpy(inner, start + 1, len);
        inner[len] = '\0';
    } else {
        strcpy(inner, s);
    }

    char current[MAX_ITEM_LEN] = "";
    int idx = 0;
    int inQuotes = 0;

    for (int i = 0; inner[i]; i++) {
        char c = inner[i];

        if (c == '"') {
            inQuotes = !inQuotes;
            continue;
        }

        if (c == ',' && !inQuotes) {
            current[idx] = '\0';
            cleanQuotes(current);
            trim(current);

            char cleaned[MAX_ITEM_LEN];
            int k = 0;
            for (int j = 0; current[j]; j++) {
                if (current[j] != '"') {
                    cleaned[k++] = current[j];
                }
            }
            cleaned[k] = '\0';
            trim(cleaned);

            if (strlen(cleaned) > 0) {
                addToStringList(list, cleaned);
            }
            idx = 0;
        } else {
            if (idx < MAX_ITEM_LEN - 1) {
                current[idx++] = c;
            }
        }
    }

    current[idx] = '\0';
    cleanQuotes(current);
    trim(current);
    char cleaned[MAX_ITEM_LEN];
    int k = 0;
    for (int j = 0; current[j]; j++) {
        if (current[j] != '"') {
            cleaned[k++] = current[j];
        }
    }
    cleaned[k] = '\0';
    trim(cleaned);

    if (strlen(cleaned) > 0) {
        addToStringList(list, cleaned);
    }
}

void parseListComma(char* s, StringList* list) {
    if (s == NULL || strlen(s) == 0) return;

    if (strchr(s, '[') && strchr(s, ']')) {
        parseListBracketed(s, list);
        return;
    }

    char* token = strtok(s, ",");
    while (token) {
        cleanQuotes(token);
        trim(token);

        char cleaned[MAX_ITEM_LEN];
        int k = 0;
        for (int j = 0; token[j]; j++) {
            if (token[j] != '"') {
                cleaned[k++] = token[j];
            }
        }
        cleaned[k] = '\0';
        trim(cleaned);

        if (strlen(cleaned) > 0) {
            addToStringList(list, cleaned);
        }
        token = strtok(NULL, ",");
    }
}

void formatList(StringList* list, char* output) {
    if (list->count == 0) {
        strcpy(output, "[]");
        return;
    }

    strcpy(output, "[");
    for (int i = 0; i < list->count; i++) {
        strcat(output, list->items[i]);
        if (i < list->count - 1) {
            strcat(output, ", ");
        }
    }
    strcat(output, "]");
}

void formatFloat(float v, char* output) {
    sprintf(output, "%.10g", v);
}

void formatPrice(float v, char* output) {
    sprintf(output, "%.2f", v);
}

void formatUserScore(float v, char* output) {
    sprintf(output, "%.1f", v);
}

void toJudgeLine(Game* g, char* output) {
    char langs[MAX_FIELD], pubs[MAX_FIELD], devs[MAX_FIELD];
    char cats[MAX_FIELD], gens[MAX_FIELD], tgs[MAX_FIELD];
    char priceStr[32], userScoreStr[32];

    formatList(&g->supportedLanguages, langs);
    formatList(&g->publishers, pubs);
    formatList(&g->developers, devs);
    formatList(&g->categories, cats);
    formatList(&g->genres, gens);
    formatList(&g->tags, tgs);
    formatPrice(g->price, priceStr);
    formatUserScore(g->userScore, userScoreStr);

    sprintf(output, "=> %d ## %s ## %s ## %d ## %s ## %s ## %d ## %s ## %d ## %s ## %s ## %s ## %s ## %s ##",
            g->id, g->name, g->releaseDate, g->estimatedOwners, priceStr, langs,
            g->metacriticScore, userScoreStr, g->achievements, pubs, devs, cats, gens, tgs);
}

void splitCSV(char* line, char fields[][MAX_FIELD], int* fieldCount) {
    *fieldCount = 0;
    int idx = 0;
    int inQuotes = 0;
    int bracketLevel = 0;

    for (int i = 0; line[i]; i++) {
        char c = line[i];

        if (c == '"') {
            inQuotes = !inQuotes;
            fields[*fieldCount][idx++] = c;
        } else if (c == '[' && !inQuotes) {
            bracketLevel++;
            fields[*fieldCount][idx++] = c;
        } else if (c == ']' && !inQuotes && bracketLevel > 0) {
            bracketLevel--;
            fields[*fieldCount][idx++] = c;
        } else if (c == ',' && !inQuotes && bracketLevel == 0) {
            fields[*fieldCount][idx] = '\0';
            (*fieldCount)++;
            idx = 0;
        } else {
            fields[*fieldCount][idx++] = c;
        }
    }
    fields[*fieldCount][idx] = '\0';
    (*fieldCount)++;
}

int compareGames(Game* g1, Game* g2) {
    comparacoes++;
    int d1, m1, y1, d2, m2, y2;
    sscanf(g1->releaseDate, "%d/%d/%d", &d1, &m1, &y1);
    sscanf(g2->releaseDate, "%d/%d/%d", &d2, &m2, &y2);

    int date1 = y1 * 10000 + m1 * 100 + d1;
    int date2 = y2 * 10000 + m2 * 100 + d2;

    if (date1 < date2) return -1;
    if (date1 > date2) return 1;

    if (g1->id < g2->id) return -1;
    if (g1->id > g2->id) return 1;
    return 0;
}

void swap(Game* a, int i, int j) {
    if (i == j) return;
    Game tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    movimentacoes += 3;
}

int partition(Game* array, int low, int high) {
    int mid = (low + high) / 2;
    Game pivot = array[mid];

    int i = low - 1;
    int j = high + 1;

    while (1) {
        do {
            i++;
        } while (compareGames(&array[i], &pivot) < 0);

        do {
            j--;
        } while (compareGames(&array[j], &pivot) > 0);

        if (i >= j) {
            return j;
        }

        swap(array, i, j);
    }
}

void quicksortRec(Game* array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quicksortRec(array, low, pi);
        quicksortRec(array, pi + 1, high);
    }
}

void quicksort(Game* array, int n) {
    comparacoes = 0;
    movimentacoes = 0;

    if (n > 0) {
        quicksortRec(array, 0, n - 1);
    }
}

int main() {
    FILE* file = fopen("/tmp/games.csv", "r");
    if (!file) {
        fprintf(stderr, "Falha ao abrir games.csv\n");
        return 1;
    }

    Game* allGames = (Game*)malloc(50000 * sizeof(Game));
    int gameCount = 0;

    char line[MAX_LINE];
    fgets(line, MAX_LINE, file); 

    while (fgets(line, MAX_LINE, file)) {
        if (strlen(line) <= 1) continue;

        char fields[20][MAX_FIELD];
        int fieldCount;
        splitCSV(line, fields, &fieldCount);

        if (fieldCount < 14) continue;

        Game g;
        g.id = parseIntOrDefault(fields[0], 0);
        cleanQuotes(fields[1]);
        strncpy(g.name, fields[1], 511);
        g.name[511] = '\0';
        normalizeDate(fields[2], g.releaseDate);
        g.estimatedOwners = parseOwners(fields[3]);
        g.price = parsePrice(fields[4]);
        g.supportedLanguages = createStringList();
        parseListBracketed(fields[5], &g.supportedLanguages);
        g.metacriticScore = parseIntOrDefault(fields[6], -1);
        g.userScore = parseUserScore(fields[7]);
        g.achievements = parseIntOrDefault(fields[8], 0);

        g.publishers = createStringList();
        g.developers = createStringList();
        g.categories = createStringList();
        g.genres = createStringList();
        g.tags = createStringList();

        if (strchr(fields[9], '[')) parseListBracketed(fields[9], &g.publishers);
        else parseListComma(fields[9], &g.publishers);

        if (strchr(fields[10], '[')) parseListBracketed(fields[10], &g.developers);
        else parseListComma(fields[10], &g.developers);

        if (strchr(fields[11], '[')) parseListBracketed(fields[11], &g.categories);
        else parseListComma(fields[11], &g.categories);

        if (strchr(fields[12], '[')) parseListBracketed(fields[12], &g.genres);
        else parseListComma(fields[12], &g.genres);

        if (strchr(fields[13], '[')) parseListBracketed(fields[13], &g.tags);
        else parseListComma(fields[13], &g.tags);

        if (g.id != 0) {
            allGames[gameCount++] = g;
        }
    }
    fclose(file);

    ListaGames* lista = newListaGames();

    char input[256];
    while (fgets(input, 256, stdin)) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "FIM") == 0) break;
        if (strlen(input) == 0) continue;

        int id = atoi(input);
        for (int i = 0; i < gameCount; i++) {
            if (allGames[i].id == id) {
                inserirFim(lista, &allGames[i]); 
                break;
            }
        }
    }

    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            int nComandos = atoi(input);
            for (int c = 0; c < nComandos; c++) {
                if (fgets(input, sizeof(input), stdin) == NULL) break;
                input[strcspn(input, "\n")] = '\0';
                if (strlen(input) == 0) { c--; continue; }

                char linhaComando[MAX_FIELD];
                strcpy(linhaComando, input);
                char* partes[3] = {NULL};
                int numPartes = 0;

                char* token = strtok(linhaComando, " ");
                while (token != NULL && numPartes < 3) {
                    partes[numPartes++] = token;
                    token = strtok(NULL, " ");
                }

                if (numPartes > 0) {
                    char* comando = partes[0];
                    Game* gameRemovido = NULL;

                    if (strcmp(comando, "II") == 0 && numPartes >= 2) {
                        int idII = atoi(partes[1]);
                        for (int i = 0; i < gameCount; i++) {
                            if (allGames[i].id == idII) { inserirInicio(lista, &allGames[i]); break; }
                        }
                    } else if (strcmp(comando, "IF") == 0 && numPartes >= 2) {
                        int idIF = atoi(partes[1]);
                        for (int i = 0; i < gameCount; i++) {
                            if (allGames[i].id == idIF) { inserirFim(lista, &allGames[i]); break; }
                        }
                    } else if (strcmp(comando, "I*") == 0 && numPartes >= 3) {
                        int posI = atoi(partes[1]);
                        int idI = atoi(partes[2]);
                        for (int i = 0; i < gameCount; i++) {
                            if (allGames[i].id == idI) { inserir(lista, &allGames[i], posI); break; }
                        }
                    } else if (strcmp(comando, "RI") == 0) {
                        gameRemovido = removerInicio(lista);
                    } else if (strcmp(comando, "RF") == 0) {
                        gameRemovido = removerFim(lista);
                    } else if (strcmp(comando, "R*") == 0 && numPartes >= 2) {
                        int posR = atoi(partes[1]);
                        gameRemovido = remover(lista, posR);
                    }

                    if (gameRemovido != NULL) {
                        printf("(R) %s\n", gameRemovido->name);
                    }
                }
            }
        }
    }

    mostrarListaAsJudgeLines(lista);

    No* current = lista->primeiro;
    while (current != NULL) {
        No* next = current->prox;
        free(current);
        current = next;
    }
    free(lista);

    for (int i = 0; i < gameCount; i++) {
        freeStringList_actual(&allGames[i].supportedLanguages);
        freeStringList_actual(&allGames[i].publishers);
        freeStringList_actual(&allGames[i].developers);
        freeStringList_actual(&allGames[i].categories);
        freeStringList_actual(&allGames[i].genres);
        freeStringList_actual(&allGames[i].tags);
    }
    free(allGames);

    return 0;
}