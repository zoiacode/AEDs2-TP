package PROVAS.EstudoQuestoesAleatorias;

class NoTrie {
    NoTrie[] filhos = new NoTrie[26];
    boolean fim;
    int frequencia;

    void inserir(String palavra) {
        NoTrie atual = raiz;

        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i);
            int idx = c - 'a';

            if (atual.filhos[idx] == null)
                atual.filhos[idx] = new NoTrie();

            atual = atual.filhos[idx];
        }

        // Chegamos no nó final da palavra
        if (!atual.fim) {
            atual.fim = true;
            atual.frequencia = 1; // primeira vez inserida
        } else {
            atual.frequencia++; // palavra repetida
        }
    }
}