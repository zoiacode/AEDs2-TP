import java.util.*;

class Carro {
    String placa, modelo, tipo, chassi;

    void ler(String linha) {
        String[] partes = linha.split(",");
        placa = partes[0];
        modelo = partes[1];
        tipo = partes[2];
        chassi = partes[3];
    }

    void imprimir() {
        System.out.println(placa + " " + modelo + " " + tipo + " " + chassi);
    }
}

public class Main {

    public static int hash(String placa, int N) {
        int soma = 0;
        for (int i = 0; i < placa.length(); i++) {
            soma += (int) placa.charAt(i);
        }
        return soma % N;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int N = Integer.parseInt(sc.nextLine());

        List<Carro>[] tabela = new LinkedList[N];
        for (int i = 0; i < N; i++) {
            tabela[i] = new LinkedList<>();
        }

        while (true) {
            String linha = sc.nextLine();

            if (linha.equals("FIM")) break;

            Carro c = new Carro();
            c.ler(linha);

            int pos = hash(c.placa, N);

            tabela[pos].add(c);
        }

        while (true) {
            String placa = sc.nextLine();

            if (placa.equals("FIM_CONSULTA")) break;

            int pos = hash(placa, N);

            boolean achei = false;

            for (Carro c : tabela[pos]) {
                if (c.placa.equals(placa)) {
                    c.imprimir();
                    achei = true;
                    break;
                }
            }

            if (!achei) {
                System.out.println("VEICULO NAO CADASTRADO");
            }
        }

        sc.close();
    }
}
