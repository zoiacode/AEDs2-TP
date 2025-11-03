package A;

public class Q3 {
    public static void main(String[] args) {
    }

    public static int[] vetorOrdenado(int[] vetA, int[] vetB) {
        int[] resp = new int[vetA.length + vetB.length];
        int i = vetA.length - 1, j = vetB.length - 1, k = 0;

        while (i >= 0 && j >= 0) {
            if (vetA[i] >= vetB[j]) {
                resp[k] = vetA[i];
                i--;
            } else {
                resp[k] = vetB[j];
                j--;
            }
            k++;
        }

        if (i == 0) {
            while (j >= 0) {
                resp[k] = vetB[j];
                j--;
                k++;
            }
        } else {
            while (i >= 0) {
                resp[k] = vetA[i];
                i--;
                k++;
            }
        }

        return resp;
    }
}
