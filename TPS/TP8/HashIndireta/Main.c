#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE 2048
#define MAX_NAME 512
#define MAX_DATE 32
#define TAM_TAB 21

typedef struct {
    int id;
    char name[MAX_NAME];
    char releaseDate[MAX_DATE];
    int estimatedOwners;
    float price;
    int metacriticScore;
    float userScore;
    int achievements;
} Game;

typedef struct Node {
    Game elemento;
    struct Node *next;
} Node;

void cleanQuotes(char *s) {
    int len = strlen(s);
    if (len >= 2 && ((s[0] == '"' && s[len-1] == '"') || (s[0] == '\'' && s[len-1] == '\''))) {
        memmove(s, s+1, len-2);
        s[len-2] = '\0';
    }
}

void trim(char *s) {
    char *start = s;
    while (isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);
    
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

int parseIntOrDefault(char *s, int def) {
    trim(s);
    cleanQuotes(s);
    if (strlen(s) == 0) return def;
    return atoi(s);
}

int parseOwners(char *s) {
    trim(s);
    cleanQuotes(s);
    char digits[64] = "";
    int idx = 0;
    for (int i = 0; s[i] != '\0' && idx < 63; i++) {
        if (isdigit((unsigned char)s[i])) digits[idx++] = s[i];
    }
    digits[idx] = '\0';
    if (strlen(digits) == 0) return 0;
    return atoi(digits);
}

float parsePrice(char *s) {
    trim(s);
    cleanQuotes(s);
    if (strlen(s) == 0) return 0.0f;
    char tmp[64];
    strncpy(tmp, s, 63); tmp[63] = '\0';
    for (char *p = tmp; *p; p++) *p = tolower((unsigned char)*p);
    if (strcmp(tmp, "free to play") == 0) return 0.0f;
    
    char clean[64] = "";
    int idx = 0;
    for (int i = 0; s[i] != '\0' && idx < 63; i++) {
        if (isdigit((unsigned char)s[i]) || s[i] == '.') clean[idx++] = s[i];
    }
    clean[idx] = '\0';
    if (idx == 0) return 0.0f;
    return atof(clean);
}

float parseUserScore(char *s) {
    trim(s);
    cleanQuotes(s);
    if (strlen(s) == 0) return -1.0f;
    char tmp[64];
    strncpy(tmp, s, 63); tmp[63] = '\0';
    for (char *p = tmp; *p; p++) *p = tolower((unsigned char)*p);
    if (strcmp(tmp, "tbd") == 0) return -1.0f;
    return atof(s);
}

void normalizeDate(char *s, char *out) {
    trim(s);
    cleanQuotes(s);
    if (strlen(s) == 0) {
        strcpy(out, "01/01/0001");
        return;
    }
    if (strlen(s) == 4) {
        int ok = 1;
        for (int i = 0; i < 4; i++) if (!isdigit((unsigned char)s[i])) ok = 0;
        if (ok) {
            sprintf(out, "01/01/%s", s);
            return;
        }
    }
    strncpy(out, s, MAX_DATE-1);
    out[MAX_DATE-1] = '\0';
}

void splitCSV(char *line, char fields[][MAX_NAME], int *count) {
    *count = 0;
    int inQuotes = 0, bracketLevel = 0, idx = 0;
    char current[MAX_NAME] = "";
    idx = 0;
    
    for (int i = 0; line[i] != '\0' && line[i] != '\n' && line[i] != '\r'; i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
            current[idx++] = c;
        } else if (c == '[' && !inQuotes) {
            bracketLevel++;
            current[idx++] = c;
        } else if (c == ']' && !inQuotes && bracketLevel > 0) {
            bracketLevel--;
            current[idx++] = c;
        } else if (c == ',' && !inQuotes && bracketLevel == 0) {
            current[idx] = '\0';
            strncpy(fields[*count], current, MAX_NAME-1);
            fields[*count][MAX_NAME-1] = '\0';
            (*count)++;
            idx = 0;
            current[0] = '\0';
        } else {
            if (idx < MAX_NAME-1) current[idx++] = c;
        }
    }
    current[idx] = '\0';
    strncpy(fields[*count], current, MAX_NAME-1);
    fields[*count][MAX_NAME-1] = '\0';
    (*count)++;
}

Game* parseGame(char *line) {
    char fields[20][MAX_NAME];
    int count = 0;
    splitCSV(line, fields, &count);
    
    if (count < 14) return NULL;
    
    Game *g = (Game*)malloc(sizeof(Game));
    if (!g) return NULL;
    g->id = parseIntOrDefault(fields[0], 0);
    
    trim(fields[1]);
    cleanQuotes(fields[1]);
    strncpy(g->name, fields[1], MAX_NAME - 1);
    g->name[MAX_NAME - 1] = '\0';
    
    normalizeDate(fields[2], g->releaseDate);
    g->estimatedOwners = parseOwners(fields[3]);
    g->price = parsePrice(fields[4]);
    g->metacriticScore = parseIntOrDefault(fields[6], -1);
    g->userScore = parseUserScore(fields[7]);
    g->achievements = parseIntOrDefault(fields[8], 0);
    
    if (g->id == 0) {
        free(g);
        return NULL;
    }
    
    return g;
}

Node* tabela[TAM_TAB];

long comparacoes = 0;

int hash(const char *name) {
    int soma = 0;
    for (size_t i = 0; i < strlen(name); i++) soma += (unsigned char)name[i];
    return soma % TAM_TAB;
}

void inserirHashIndirect(Game *g) {
    if (g == NULL) return;
    int pos = hash(g->name);
    Node *novo = (Node*)malloc(sizeof(Node));
    if (!novo) return;
    novo->elemento = *g; 
    novo->next = NULL;
    if (tabela[pos] == NULL) {
        tabela[pos] = novo;
    } else {
        Node *p = tabela[pos];
        while (p->next != NULL) p = p->next;
        p->next = novo;
    }
}

int pesquisarComImpressao(const char *nome) {
    int pos = hash(nome);
    Node *p = tabela[pos];
    while (p != NULL) {
        comparacoes++;
        if (strcmp(p->elemento.name, nome) == 0) {
            printf("%s:  (Posicao: %d) SIM\n", nome, pos);
            return 1;
        }
        p = p->next;
    }
    printf("%s:  (Posicao: %d) NAO\n", nome, pos);
    return 0;
}

void liberarTabela() {
    for (int i = 0; i < TAM_TAB; i++) {
        Node *p = tabela[i];
        while (p) {
            Node *tmp = p->next;
            free(p);
            p = tmp;
        }
        tabela[i] = NULL;
    }
}


int main() {
    char csvPath[] = "/tmp/games.csv";
    FILE *fp = fopen(csvPath, "r");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir CSV\n");
        return 1;
    }
    
    Game *allGames[50000];
    int gameCount = 0;
    
    char line[MAX_LINE];
    if (!fgets(line, MAX_LINE, fp)) {
        fclose(fp);
        return 1;
    }
    
    while (fgets(line, MAX_LINE, fp)) {
        Game *g = parseGame(line);
        if (g != NULL) {
            allGames[gameCount++] = g;
            if (gameCount >= 50000) break;
        }
    }
    fclose(fp);
    
    for (int i = 0; i < TAM_TAB; i++) tabela[i] = NULL;
    
    char input[512];
    while (1) {
        if (!fgets(input, sizeof(input), stdin)) break;
        trim(input);
        if (strcmp(input, "FIM") == 0) break;
        if (strlen(input) == 0) continue;
        
        int id = atoi(input);
        for (int i = 0; i < gameCount; i++) {
            if (allGames[i]->id == id) {
                inserirHashIndirect(allGames[i]); 
                break;
            }
        }
    }
    
    clock_t inicio = clock();
    long totalComparacoes = 0;
    
    while (1) {
        if (!fgets(input, sizeof(input), stdin)) break;
        trim(input);
        if (strcmp(input, "FIM") == 0) break;
        if (strlen(input) == 0) continue;
    
        comparacoes = 0;
        pesquisarComImpressao(input);
        totalComparacoes += comparacoes;
    }
    
    clock_t fim = clock();
    long tempoExecucao = ((fim - inicio) * 1000000L) / CLOCKS_PER_SEC; /* microssegundos */
    
    char matricula[] = "840005";
    char nomeArquivoLog[64];
    sprintf(nomeArquivoLog, "%s_hashIndireta.txt", matricula);
    
    FILE *logFile = fopen(nomeArquivoLog, "w");
    if (logFile) {
        fprintf(logFile, "%s\t%ld\t%ld\n", matricula, totalComparacoes, tempoExecucao);
        fclose(logFile);
    } else {
        fprintf(stderr, "Erro ao escrever log\n");
    }
    
    liberarTabela();
    for (int i = 0; i < gameCount; i++) free(allGames[i]);
    
    return 0;
}
