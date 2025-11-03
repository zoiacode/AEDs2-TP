typedef struct Celula {
    int elemento;
    struct Celula *inf, *sup, *esq, *dir;
} Celula;

typedef struct {
    int linhas, colunas;
    Celula *inicio;
} Matriz

void removerUltimaColuna(Matriz *matriz) {
    if (matriz == NULL || matriz->colunas == 0 || matriz->inicio == NULL) {
        return;
    }
    
    if (matriz->colunas == 1) {
        Celula *linhaAtual = matriz->inicio;
        
        while (linhaAtual != NULL) {
            Celula *proximaLinha = linhaAtual->inf;
            free(linhaAtual);
            linhaAtual = proximaLinha;
        }
        
        matriz->inicio = NULL;
        matriz->linhas = 0;
        matriz->colunas = 0;
        return;
    }
    
    Celula *linhaAtual = matriz->inicio;
    
    while (linhaAtual != NULL) {
        Celula *penultimaColuna = linhaAtual;
        Celula *ultimaColuna = linhaAtual->dir;
        
        while (ultimaColuna->dir != NULL) {
            penultimaColuna = ultimaColuna;
            ultimaColuna = ultimaColuna->dir;
        }
        
        penultimaColuna->dir = NULL;
        
        if (ultimaColuna->inf != NULL) {
            ultimaColuna->inf->sup = NULL;
        }
        
        free(ultimaColuna);

        linhaAtual = linhaAtual->inf;
    }
    
    matriz->colunas--;
}