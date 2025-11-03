#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

#define MAX_LINE 8192
#define MAX_FIELD 2048
#define MAX_LIST 100
#define MAX_GAMES 50000

typedef struct {
    char **items;
    int count;
} StringList;

typedef struct {
    int id;
    char *name;
    char releaseDate[11]; // dd/MM/yyyy
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

// ===== Helper Functions =====
static char* my_strndup(const char* s, size_t n) {
    char* p = (char*)malloc(n + 1);
    if (!p) return NULL;
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

static int month_from_abbrev(const char* m) {
    char buf[4];
    if (!m || strlen(m) < 3) return 0;
    buf[0] = tolower((unsigned char)m[0]);
    buf[1] = tolower((unsigned char)m[1]);
    buf[2] = tolower((unsigned char)m[2]);
    buf[3] = '\0';
    if (!strcmp(buf,"jan")) return 1;
    if (!strcmp(buf,"feb")) return 2;
    if (!strcmp(buf,"mar")) return 3;
    if (!strcmp(buf,"apr")) return 4;
    if (!strcmp(buf,"may")) return 5;
    if (!strcmp(buf,"jun")) return 6;
    if (!strcmp(buf,"jul")) return 7;
    if (!strcmp(buf,"aug")) return 8;
    if (!strcmp(buf,"sep")) return 9;
    if (!strcmp(buf,"oct")) return 10;
    if (!strcmp(buf,"nov")) return 11;
    if (!strcmp(buf,"dec")) return 12;
    return 0;
}

// ===== String List Functions =====
void initList(StringList *list) {
    list->items = malloc(MAX_LIST * sizeof(char*));
    list->count = 0;
}

void addToList(StringList *list, const char *item) {
    if (list->count < MAX_LIST) {
        list->items[list->count] = strdup(item);
        list->count++;
    }
}

void freeList(StringList *list) {
    for (int i = 0; i < list->count; i++) {
        free(list->items[i]);
    }
    free(list->items);
}

// ===== Parsing Helper Functions =====
char* cleanQuotes(char *s) {
    if (!s) return strdup("");
    while (isspace(*s)) s++;
    int len = strlen(s);
    while (len > 0 && isspace(s[len-1])) len--;
    if (len >= 2 && ((s[0] == '"' && s[len-1] == '"') || (s[0] == '\'' && s[len-1] == '\''))) {
        s++; len -= 2;
    }
    char *result = malloc(len + 1);
    strncpy(result, s, len);
    result[len] = '\0';
    return result;
}

int parseIntOrDefault(char *s, int def) {
    char *clean = cleanQuotes(s);
    if (strlen(clean) == 0) {
        free(clean);
        return def;
    }
    int val = atoi(clean);
    free(clean);
    return val;
}

float parseFloatOrDefault(char *s, float def) {
    char *clean = cleanQuotes(s);
    if (strlen(clean) == 0) {
        free(clean);
        return def;
    }
    float val = atof(clean);
    free(clean);
    return val;
}

int parseOwners(char *s) {
    char *clean = cleanQuotes(s);
    char digits[256] = "";
    int idx = 0;
    for (int i = 0; clean[i] && idx < 255; i++) {
        if (isdigit(clean[i])) digits[idx++] = clean[i];
    }
    digits[idx] = '\0';
    free(clean);
    return strlen(digits) > 0 ? atoi(digits) : 0;
}

float parsePrice(char *s) {
    char *clean = cleanQuotes(s);
    if (strlen(clean) == 0 || strcasecmp(clean, "Free to Play") == 0) {
        free(clean);
        return 0.0f;
    }
    char *p = clean;
    while (*p) {
        if (*p == '$' || *p == ',') memmove(p, p+1, strlen(p));
        else p++;
    }
    float val = atof(clean);
    free(clean);
    return val;
}

float parseUserScore(char *s) {
    char *clean = cleanQuotes(s);
    if (strlen(clean) == 0 || strcasecmp(clean, "tbd") == 0) {
        free(clean);
        return -1.0f;
    }
    float val = atof(clean);
    free(clean);
    return val;
}

void normalizeDate(char *s, char *out) {
    strcpy(out, "01/01/0001");

    char *clean = cleanQuotes(s);
    if (!clean || clean[0] == '\0') {
        if (clean) free(clean);
        return;
    }

    // 1) Se for apenas ano
    int onlyYear = 1;
    for (int i = 0; clean[i]; i++) {
        if (!isdigit((unsigned char)clean[i]) && !isspace((unsigned char)clean[i])) {
            onlyYear = 0; break;
        }
    }
    if (onlyYear) {
        int y = 0; int cnt = 0;
        for (int i = 0; clean[i]; i++) {
            if (isdigit((unsigned char)clean[i])) {
                y = y*10 + (clean[i]-'0');
                cnt++;
                if (cnt == 4) break;
            }
        }
        if (cnt >= 4 && y > 0) {
            sprintf(out, "01/01/%04d", y);
            free(clean);
            return;
        }
    }

    // 2) Tentar "MMM d, yyyy" | "MMM dd, yyyy"
    {
        char mmm[16]; int d=0, y=0;
        if (sscanf(clean, " %15s %d , %d", mmm, &d, &y) == 3) {
            int m = month_from_abbrev(mmm);
            if (m >= 1 && m <= 12 && d >= 1 && d <= 31 && y >= 1) {
                sprintf(out, "%02d/%02d/%04d", d, m, y);
                free(clean);
                return;
            }
        }
    }
    // 3) Tentar "d MMM, yyyy" | "dd MMM, yyyy"
    {
        char mmm[16]; int d=0, y=0;
        if (sscanf(clean, " %d %15s , %d", &d, mmm, &y) == 3) {
            int m = month_from_abbrev(mmm);
            if (m >= 1 && m <= 12 && d >= 1 && d <= 31 && y >= 1) {
                sprintf(out, "%02d/%02d/%04d", d, m, y);
                free(clean);
                return;
            }
        }
    }
    // 4) Tentar "MMM yyyy" (assume dia 1)
    {
        char mmm[16]; int y=0;
        if (sscanf(clean, " %15s %d", mmm, &y) == 2) {
            int m = month_from_abbrev(mmm);
            if (m >= 1 && m <= 12 && y >= 1) {
                sprintf(out, "%02d/%02d/%04d", 1, m, y);
                free(clean);
                return;
            }
        }
    }
    // 5) Tentar pegar dígitos "d m yyyy" ou "m yyyy"
    {
        char digits[128] = {0};
        int j = 0;
        for (int i = 0; clean[i] && j < 120; i++) {
            if (isdigit((unsigned char)clean[i])) digits[j++] = clean[i];
            else digits[j++] = ' ';
        }
        digits[j] = '\0';
        char norm[128] = {0};
        int k = 0, inSpace = 0;
        for (int i = 0; digits[i] && k < 120; i++) {
            if (isspace((unsigned char)digits[i])) {
                if (!inSpace) { norm[k++] = ' '; inSpace = 1; }
            } else {
                norm[k++] = digits[i];
                inSpace = 0;
            }
        }
        norm[k] = '\0';

        int a=0,b=0,c=0;
        if (sscanf(norm, " %d %d %d", &a, &b, &c) == 3) {
            if (a>=1 && a<=31 && b>=1 && b<=12 && c>=1) {
                sprintf(out, "%02d/%02d/%04d", a, b, c);
                free(clean);
                return;
            }
        }
        if (sscanf(norm, " %d %d", &a, &b) == 2) {
            if (a>=1 && a<=12 && b>=1) {
                sprintf(out, "%02d/%02d/%04d", 1, a, b);
                free(clean);
                return;
            }
        }
    }

    free(clean);
    strcpy(out, "01/01/0001");
}

void parseListBracketed(char *s, StringList *list) {
    if (!s || strlen(s) == 0) return;
    char *start = strchr(s, '[');
    char *end = strrchr(s, ']');
    if (!start || !end || end <= start) return;
    
    char *inner = my_strndup(start + 1, (size_t)(end - start - 1));
    char *token = strtok(inner, ",");
    while (token) {
        char *clean = cleanQuotes(token);
        if (strlen(clean) > 0) addToList(list, clean);
        free(clean);
        token = strtok(NULL, ",");
    }
    free(inner);
}

void parseListComma(char *s, StringList *list) {
    if (!s || strlen(s) == 0) return;
    if (strchr(s, '[') && strchr(s, ']')) {
        parseListBracketed(s, list);
        return;
    }
    char *copy = strdup(s);
    char *token = strtok(copy, ",");
    while (token) {
        char *clean = cleanQuotes(token);
        if (strlen(clean) > 0) addToList(list, clean);
        free(clean);
        token = strtok(NULL, ",");
    }
    free(copy);
}

void formatList(StringList *list, char *out) {
    if (list->count == 0) {
        strcpy(out, "[]");
        return;
    }
    strcpy(out, "[");
    for (int i = 0; i < list->count; i++) {
        strcat(out, list->items[i]);
        if (i < list->count - 1) strcat(out, ", ");
    }
    strcat(out, "]");
}

void formatFloat(float v, char *out) {
    sprintf(out, "%.10g", v);
}

// ===== CSV Parsing =====
int splitCSV(char *line, char fields[][MAX_FIELD]) {
    int fieldCount = 0;
    char cur[MAX_FIELD] = "";
    int curIdx = 0;
    int inQuotes = 0;
    int bracketLevel = 0;
    
    for (int i = 0; line[i]; i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
            cur[curIdx++] = c;
        } else if (c == '[' && !inQuotes) {
            bracketLevel++;
            cur[curIdx++] = c;
        } else if (c == ']' && !inQuotes && bracketLevel > 0) {
            bracketLevel--;
            cur[curIdx++] = c;
        } else if (c == ',' && !inQuotes && bracketLevel == 0) {
            cur[curIdx] = '\0';
            strcpy(fields[fieldCount++], cur);
            curIdx = 0;
        } else {
            cur[curIdx++] = c;
        }
    }
    cur[curIdx] = '\0';
    strcpy(fields[fieldCount++], cur);
    return fieldCount;
}

// ===== Game Functions =====
void initGame(Game *g) {
    g->id = 0;
    g->name = NULL;
    strcpy(g->releaseDate, "01/01/0001");
    g->estimatedOwners = 0;
    g->price = 0.0f;
    g->metacriticScore = -1;
    g->userScore = -1.0f;
    g->achievements = 0;
    initList(&g->supportedLanguages);
    initList(&g->publishers);
    initList(&g->developers);
    initList(&g->categories);
    initList(&g->genres);
    initList(&g->tags);
}

void freeGame(Game *g) {
    if (g->name) free(g->name);
    freeList(&g->supportedLanguages);
    freeList(&g->publishers);
    freeList(&g->developers);
    freeList(&g->categories);
    freeList(&g->genres);
    freeList(&g->tags);
}

void printGame(Game *g) {
    char buf[MAX_LINE * 2];
    char temp[MAX_LINE];
    
    sprintf(buf, "=> %d ## %s ## %s ## %d ## ", g->id, g->name, g->releaseDate, g->estimatedOwners);
    formatFloat(g->price, temp);
    strcat(buf, temp);
    strcat(buf, " ## ");
    formatList(&g->supportedLanguages, temp);
    strcat(buf, temp);
    sprintf(temp, " ## %d ## ", g->metacriticScore);
    strcat(buf, temp);
    formatFloat(g->userScore, temp);
    strcat(buf, temp);
    sprintf(temp, " ## %d ## ", g->achievements);
    strcat(buf, temp);
    formatList(&g->publishers, temp);
    strcat(buf, temp);
    strcat(buf, " ## ");
    formatList(&g->developers, temp);
    strcat(buf, temp);
    strcat(buf, " ## ");
    formatList(&g->categories, temp);
    strcat(buf, temp);
    strcat(buf, " ## ");
    formatList(&g->genres, temp);
    strcat(buf, temp);
    strcat(buf, " ## ");
    formatList(&g->tags, temp);
    strcat(buf, temp);
    strcat(buf, " ##");
    
    printf("%s\n", buf);
}

// ===== Main =====
int main() {
    Game *games = malloc(MAX_GAMES * sizeof(Game));
    int gameCount = 0;
    
    FILE *fp = fopen("/tmp/games.csv", "r");
    if (!fp) {
        fprintf(stderr, "Falha ao abrir games.csv\n");
        return 1;
    }
    
    char line[MAX_LINE];
    fgets(line, MAX_LINE, fp); // skip header
    
    while (fgets(line, MAX_LINE, fp)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;
        
        char fields[20][MAX_FIELD];
        int fieldCount = splitCSV(line, fields);
        if (fieldCount < 14) continue;
        
        Game g;
        initGame(&g);
        g.id = parseIntOrDefault(fields[0], 0);
        g.name = cleanQuotes(fields[1]);
        normalizeDate(fields[2], g.releaseDate);
        g.estimatedOwners = parseOwners(fields[3]);
        g.price = parsePrice(fields[4]);
        parseListBracketed(fields[5], &g.supportedLanguages);
        g.metacriticScore = parseIntOrDefault(fields[6], -1);
        g.userScore = parseUserScore(fields[7]);
        g.achievements = parseIntOrDefault(fields[8], 0);
        parseListComma(fields[9], &g.publishers);
        parseListComma(fields[10], &g.developers);
        parseListComma(fields[11], &g.categories);
        parseListComma(fields[12], &g.genres);
        parseListComma(fields[13], &g.tags);
        
        if (g.id != 0) games[gameCount++] = g;
    }
    fclose(fp);
    
    char input[100];
    while (fgets(input, 100, stdin)) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "FIM") == 0) break;
        if (strlen(input) == 0) continue;
        
        int id = atoi(input);
        for (int i = 0; i < gameCount; i++) {
            if (games[i].id == id) {
                printGame(&games[i]);
                break;
            }
        }
    }
    
    for (int i = 0; i < gameCount; i++) {
        freeGame(&games[i]);
    }
    free(games);
    
    return 0;
}