class Celula {
    public String palavra;
    public Celula prox;
}

class No {
    public char letra;
    public No esq, dir;
    public Celula primeiro, ultimo;
}

class Dicionario {
    private No raiz;

    public void Inserir(String palavra) {
        if (palavra == null || palavra.isEmpty()) {
            return;
        }

        char primeiraLetra = Character.toLowerCase(palavra.charAt(0));
        raiz = inserirNo(raiz, primeiraLetra, palavra);
    }

    private No inserirNo(No no, char letra, String palavra) {
        if (no == null) {
            no = new No();
            no.letra = letra;
            no.esq = null;
            no.dir = null;
            no.primeiro = no.ultimo = null;
            inserirPalavra(no, palavra);
            return no;
        }

        if (letra < no.letra) {
            no.esq = inserirNo(no.esq, letra, palavra);
        } else if (letra > no.letra) {
            no.dir = inserirNo(no.dir, letra, palavra);
        } else {
            inserirPalavra(no, palavra);
        }

        return no;
    }

    private void inserirPalavra(No no, String palavra) {
        Celula nova = new Celula();
        nova.palavra = palavra;
        nova.prox = null;

        if (no.primeiro == null) {
            no.primeiro = no.ultimo = nova;
            return;
        }

        if (palavra.compareToIgnoreCase(no.primeiro.palavra) < 0) {
            nova.prox = no.primeiro;
            no.primeiro = nova;
            return;
        }

        Celula atual = no.primeiro;
        while (atual.prox != null && palavra.compareToIgnoreCase(atual.prox.palavra) > 0) {
            atual = atual.prox;
        }

        if (atual.palavra.equalsIgnoreCase(palavra) ||
                (atual.prox != null && atual.prox.palavra.equalsIgnoreCase(palavra))) {
            return;
        }

        nova.prox = atual.prox;
        atual.prox = nova;

        if (nova.prox == null) {
            no.ultimo = nova;
        }
    }
}