#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
void selectionSort(Game* array, int n);

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

void freeStringList(StringList* list) {
    for (int i = 0; i < list->count; i++) {
        free(list->items[i]);
    }
    free(list->items);
}

void cleanQuotes(char* s) {
    if (s == NULL) return;
    
    while (*s && isspace(*s)) s++;
    
    int len = strlen(s);
    if (len == 0) return;
    
    while (len > 0 && isspace(s[len - 1])) {
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
        if (isdigit(s[i])) {
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

    if (strlen(s) == 4 && isdigit(s[0]) && isdigit(s[1]) && isdigit(s[2]) && isdigit(s[3])) {
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
        if (isdigit(s[i])) {
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
    while (*start && isspace(*start)) start++;

    char* end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;

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
    int cmp = strcmp(g1->name, g2->name);
    
    if (cmp < 0) return -1;
    if (cmp > 0) return 1;
    
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

void selectionSort(Game* array, int n) {
    comparacoes = 0;
    movimentacoes = 0;
    
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        
        for (int j = i + 1; j < n; j++) {
            if (compareGames(&array[j], &array[minIdx]) < 0) {
                minIdx = j;
            }
        }

        if (minIdx != i) {
            swap(array, i, minIdx);
        }
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
    
    Game* selectedGames = (Game*)malloc(gameCount * sizeof(Game));
    int selectedCount = 0;
    
    char input[256];
    while (fgets(input, 256, stdin)) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "FIM") == 0) break;
        if (strlen(input) == 0) continue;
        
        int id = atoi(input);
        for (int i = 0; i < gameCount; i++) {
            if (allGames[i].id == id) {
                selectedGames[selectedCount++] = allGames[i];
                break;
            }
        }
    }
    
    if (selectedCount == 0) {
        free(allGames);
        free(selectedGames);
        return 0;
    }
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    selectionSort(selectedGames, selectedCount);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long long tempoExecucao = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    
    char output[MAX_LINE * 2];
    for (int i = 0; i < selectedCount; i++) {
        toJudgeLine(&selectedGames[i], output);
        printf("%s\n", output);
    }
    
    FILE* logFile = fopen("840005_mergesort.txt", "w");
    if (logFile) {
        fprintf(logFile, "840005\t%lld\t%lld\t%lld\n", comparacoes, movimentacoes, tempoExecucao);
        fclose(logFile);
    }
    
    free(allGames);
    free(selectedGames);
    
    return 0;
}