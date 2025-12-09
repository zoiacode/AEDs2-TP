package PROVAS.EstudoQuestoesAleatorias;

public class implementacaoMetodoIsAVL {
    boolean isAVL(No x) {
        if (x == null)
            return true;

        // 1. Verifica se é BST
        if (!isBST(x, Integer.MIN_VALUE, Integer.MAX_VALUE))
            return false;

        // 2. Verifica se está balanceada (AVL)
        if (Math.abs(altura(x.esq) - altura(x.dir)) > 1)
            return false;

        return isAVL(x.esq) && isAVL(x.dir);
    }

    boolean isBST(No x, int min, int max) {
        if (x == null)
            return true;

        if (x.valor <= min || x.valor >= max)
            return false;

        return isBST(x.esq, min, x.valor)
                && isBST(x.dir, x.valor, max);
    }

    int altura(No x) {
        if (x == null)
            return 0;
        return 1 + Math.max(altura(x.esq), altura(x.dir));
    }

}
