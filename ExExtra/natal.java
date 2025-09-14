package ExExtra;

import java.util.Scanner;

class Presente {
    String nome;
    double preco;
    int preferencia;
}

public class natal {
    public static void main(String[] args) {
        Scanner scanf = new Scanner(System.in);

        while (scanf.hasNext()) {

            String value = scanf.nextLine();
            String[] splitted = value.split(" ");

            String nome = splitted[0];
            int qntdPresente = Integer.parseInt(splitted[1]);
            Presente[] presente = new Presente[qntdPresente];

            for (int i = 0; i < qntdPresente; i++) {
                presente[i] = new Presente();
                presente[i].nome = scanf.nextLine();
                String[] splitted2 = scanf.nextLine().split(" ");
                presente[i].preco = Double.parseDouble(splitted2[0]);
                presente[i].preferencia = Integer.parseInt(splitted2[1]);
            }

            BubbleSort(presente);

            printArray(presente, nome);

            if (scanf.hasNext())
                scanf.nextLine();
        }

        scanf.close();
    }

    public static void BubbleSort(Presente[] array) {
        int n = array.length;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (array[j].preferencia > array[j + 1].preferencia) {
                    // Troca array[j] e array[j+1]
                    Presente temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                } else if (array[j].preferencia == array[j + 1].preferencia) {
                    if (array[j].preco < array[j + 1].preco) {
                        // Troca array[j] e array[j+1]
                        Presente temp = array[j];
                        array[j] = array[j + 1];
                        array[j + 1] = temp;
                    } else if (array[j].preferencia == array[j + 1].preferencia
                            && array[j].preco == array[j + 1].preco) {
                        if (array[j].nome.toLowerCase().compareTo(array[j + 1].nome.toLowerCase()) < 0) {
                            // Troca array[j] e array[j+1]
                            Presente temp = array[j];
                            array[j] = array[j + 1];
                            array[j + 1] = temp;
                        }
                    }
                }
            }
        }
    }

    public static void printArray(Presente[] array, String nome) {
        int n = array.length;
        System.out.println("Lista de " + nome);
        for (int i = n - 1; i >= 0; i--)
            System.out.println(array[i].nome + " - " + array[i].preco);

        System.out.println();
    }
}