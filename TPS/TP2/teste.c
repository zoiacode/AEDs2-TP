#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Verifica se string só tem vogais
bool soVogais(char s[]) {
    char vogais[] = "aeiouAEIOU";
    int n = strlen(s);
    if (n == 0) return false;
    for (int i = 0; i < n; i++) {
        bool ehVogal = false;
        for (int j = 0; j < 10; j++) {
            if (s[i] == vogais[j]) {
                ehVogal = true;
                break;
            }
        }
        if (!ehVogal) return false;
    }
    return true;
}

// Verifica se string só tem consoantes
bool soConsoantes(char s[]) {
    char vogais[] = "aeiouAEIOU";
    int n = strlen(s);
    if (n == 0) return false;
    for (int i = 0; i < n; i++) {
        if (!isalpha(s[i])) return false;
        for (int j = 0; j < 10; j++) {
            if (s[i] == vogais[j]) return false;
        }
    }
    return true;
}

// Verifica se é inteiro
bool inteiro(char s[]) {
    int n = strlen(s);
    if (n == 0) return false;
    int i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i == n) return false;
    for (; i < n; i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

// Verifica se é real
bool real(char s[]) {
    int n = strlen(s);
    if (n == 0) return false;
    int i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i == n) return false;

    bool ponto = false, digito = false;
    for (; i < n; i++) {
        if (isdigit(s[i])) {
            digito = true;
        } else if (s[i] == '.' || s[i] == ',') {
            if (ponto) return false; // já tinha separador
            ponto = true;
        } else {
            return false;
        }
    }
    return digito && ponto;
}

int main() {
    char entrada[100];

    while (1) {
        if (scanf("%99s", entrada) != 1) break;

        if (strcmp(entrada, "FIM") == 0) break;

        bool x1 = soVogais(entrada);
        bool x2 = soConsoantes(entrada);
        bool x3 = inteiro(entrada);
        bool x4 = real(entrada);

        printf("%s %s %s %s\n",
               x1 ? "SIM" : "NAO",
               x2 ? "SIM" : "NAO",
               x3 ? "SIM" : "NAO",
               x4 ? "SIM" : "NAO");
    }

    return 0;
}
