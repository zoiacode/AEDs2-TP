package Prova1AntigaJava;

import java.util.Scanner;

public class Q1 {

    public static void main(String[] args) {
        Scanner scanf = new Scanner(System.in);

        int contadorParenteses = 0;
        int contadorColchetes = 0;
        int contadorChaves = 0;
        boolean jaEra = false;

        int quantidade = Integer.parseInt(scanf.nextLine());
        String[] array = new String[quantidade];
        for (int i = 0; i < quantidade; i++) {
            array[i] = scanf.nextLine();
        }

        for (int i = 0; i < quantidade; i++) {
            for (int j = 0; j < array[i].length(); j++) {
                char caractere = array[i].charAt(j);
                if (caractere == '(') {
                    contadorParenteses++;
                } else if (caractere == ')') {
                    contadorParenteses--;
                    if (contadorParenteses < 0) {
                        jaEra = true;
                    }
                } else if (caractere == '[') {
                    contadorColchetes++;
                } else if (caractere == ']') {
                    contadorColchetes--;
                    if (contadorColchetes < 0) {
                        jaEra = true;
                    }
                } else if (caractere == '{') {
                    contadorChaves++;
                } else if (caractere == '}') {
                    contadorChaves--;
                    if (contadorChaves < 0) {
                        jaEra = true;
                    }
                }
            }

            if (!jaEra) {
                if (contadorParenteses == 0 && contadorColchetes == 0 && contadorChaves == 0) {
                    System.out.println("S");
                } else {
                    System.out.println("N");
                }
            } else {
                System.out.println("N");
            }

            jaEra = false;
            contadorParenteses = 0;
            contadorColchetes = 0;
            contadorChaves = 0;

        }

        scanf.close();
    }
}
