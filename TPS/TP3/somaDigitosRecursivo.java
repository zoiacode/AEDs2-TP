import java.util.Scanner;

public class somaDigitosRecursivo {
    public static void main(String[] args) {
        Scanner scanf = new Scanner(System.in);

        String valor;
        int resultado;
        do {
            valor = scanf.nextLine();
            if (!stop(valor)) {
                int numero = Integer.parseInt(valor);
                resultado = somaDigitos(numero);
                System.out.println(resultado);
            }
            resultado = 0;
        } while (!stop(valor));

        scanf.close();
    }

    public static int somaDigitos(int n) {
        if (n == 0) {
            return 0;
        } else {
            return (n % 10) + somaDigitos(n / 10);
        }
    }

    public static boolean stop(String valor) {
        if (valor.length() == 3 && valor.charAt(0) == 'F' && valor.charAt(1) == 'I' && valor.charAt(2) == 'M') {
            return true;
        } else
            return false;
    }
}