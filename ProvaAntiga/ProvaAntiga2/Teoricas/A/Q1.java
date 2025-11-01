package ProvaAntiga2;

class Lista {
    CelulaLista inicio;
    CelulaLista fim;

    CelulaLista maiorPilha() {
        CelulaLista maior = inicio;
        CelulaLista atual = inicio;

        while (atual != null) {
            if (tamanhoPilha(atual.topo) > tamanhoPilha(maior.topo)) {
                maior = atual;
            }
            atual = atual.prox;
        }

        return maior;
    }

    int tamanhoPilha(CelulaPilha topo) {
        int tamanho = 0;
        CelulaPilha atual = topo;

        while (atual != null) {
            tamanho++;
            atual = atual.prox;
        }

        return tamanho;
    }
}

class CelulaLista {
    CelulaPilha topo;
    CelulaLista prox;
}

class CelulaPilha {
    int elemento;
    CelulaPilha prox;
}
