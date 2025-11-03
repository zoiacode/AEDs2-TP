package ProvaAntiga2.Teoricas.A;

import java.util.*;

class No2 {
    String palavra;
    No2 esq, dir;
}

class No {
    char letra;
    No esq, dir;
    No2 raiz;
}

class Arvore {
    No raiz;

    int contarPalavras(String padrao) {
        int resp;
        if (padrao == null || padrao.length() == 0) {
            return 0;
        }

        char primeiraLetra = padrao.charAt(0);
        int tamanho = padrao.length();

        No noLetra = buscarLetra(raiz, primeiraLetra);

        if (noLetra == null || noLetra.raiz == null) {
            return 0;
        }

        resp = contarPorTamanho(noLetra.raiz, tamanho);
        return resp;
    }

    private No buscarLetra(No atual, char letra) {
        if (atual == null) {
            return null;
        }

        if (atual.letra == letra) {
            return atual;
        }

        if (letra < atual.letra) {
            return buscarLetra(atual.esq, letra);
        } else {
            return buscarLetra(atual.dir, letra);
        }
    }

    private int contarPorTamanho(No2 atual, int tamanho) {
        if (atual == null) {
            return 0;
        }

        int contador = 0;

        if (atual.palavra != null && atual.palavra.length() == tamanho) {
            contador += 1;
        }

        contador += contarPorTamanho(atual.esq, tamanho);
        contador += contarPorTamanho(atual.dir, tamanho);

        return contador;
    }
}
