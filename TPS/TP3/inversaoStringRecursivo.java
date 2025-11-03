import java.util.Scanner;

public class inversaoStringRecursivo {
    public static void main(String[] args) {
        String valor;
        String resultado;
        Scanner scanf = new Scanner(System.in);

        do {
            // valor = MyIO.readLine();
            valor = scanf.nextLine();
            if (!stop(valor)) {
                resultado = inversaoString(valor);
                // MyIO.println(resultado);
                System.out.println(resultado);
            }
            resultado = " ";
        } while (!stop(valor));
        scanf.close();
    }

    public static boolean stop(String valor) {
        if (valor.length() == 3 && valor.charAt(0) == 'F' && valor.charAt(1) == 'I' && valor.charAt(2) == 'M') {
            return true;
        } else
            return false;
    }

    public static String inversaoString(String s) {
        if (s.isEmpty()) {
            return s;
        } else {
            return inversaoString(s.substring(1)) + s.charAt(0);
        }
    }
}
