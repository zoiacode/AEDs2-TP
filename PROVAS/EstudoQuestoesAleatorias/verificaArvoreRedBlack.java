package PROVAS.EstudoQuestoesAleatorias;

public class verificaArvoreRedBlack {
    int menorCaminho(No x) {
        if (x == null)
            return 0;

        return 1 + Math.min(menorCaminho(x.esq), menorCaminho(x.dir));
    }

    int maiorCaminho(No x) {
        if (x == null)
            return 0;

        return 1 + Math.max(maiorCaminho(x.esq), maiorCaminho(x.dir));
    }

    boolean verificaArvore() {
        return verificaNo(raiz);
    }

    boolean verificaNo(No x) {
        if (x == null)
            return true;

        int menor = menorCaminho(x);
        int maior = maiorCaminho(x);

        // Verifica a propriedade
        if (maior > 2 * menor)
            return false;

        // Verifica recursivamente para toda a árvore
        return verificaNo(x.esq) && verificaNo(x.dir);
    }

}
