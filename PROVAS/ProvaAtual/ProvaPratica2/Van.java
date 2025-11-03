import java.util.*;

class Estudante {
    public String nome;
    public char direcao;
    public int distancia;

    Estudante(String nome, char direcao, int distancia) {
        this.nome = nome;
        this.direcao = direcao;
        this.distancia = distancia;
    }
}

public class Van {
    public static void main(String[] args) {
        Scanner scanf = new Scanner(System.in);
        int capacidade = scanf.nextInt();
        scanf.nextLine();
        Estudante[] estudantes = new Estudante[capacidade];
        String[] frase = new String[capacidade];

        for (int i = 0; i < capacidade; i++) {
            frase[i] = scanf.nextLine();
            String[] partes = frase[i].split(" ");
            String nome = partes[0];
            String direcao = partes[1];
            String distancia = partes[2];
            estudantes[i] = new Estudante(nome, direcao.charAt(0), Integer.parseInt(distancia));
        }

        BubbleSort(estudantes);

        scanf.close();
    }

    public static void BubbleSort(Estudante[] estudantes) {
        if (estudantes == null) {
            return;
        }

        int n = estudantes.length;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (estudantes[j].distancia > estudantes[j + 1].distancia) {
                    Estudante temp = estudantes[j];
                    estudantes[j] = estudantes[j + 1];
                    estudantes[j + 1] = temp;
                } else if (estudantes[j].distancia == estudantes[j + 1].distancia) {
                    if (estudantes[j].direcao > estudantes[j + 1].direcao) {
                        Estudante temp = estudantes[j];
                        estudantes[j] = estudantes[j + 1];
                        estudantes[j + 1] = temp;
                    } else if (estudantes[j].direcao == estudantes[j + 1].direcao) {
                        if (estudantes[j].nome.compareTo(estudantes[j + 1].nome) > 0) {
                            Estudante temp = estudantes[j];
                            estudantes[j] = estudantes[j + 1];
                            estudantes[j + 1] = temp;
                        }
                    }
                }
            }

        }

        for (int i = 0; i < n; i++) {
            System.out.println(estudantes[i].nome);
        }
    }
}
