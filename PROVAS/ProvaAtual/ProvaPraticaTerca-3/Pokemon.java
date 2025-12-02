import java.util.*;

public class Pokemon {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        
        Set<String> capturados = new HashSet<>();

        for (int i = 0; i < N; i++) {
            capturados.add(sc.next());
        }

        int faltam = 151 - capturados.size();

        System.out.println("Falta(m) " + faltam + " pokemon(s)");
    }
}
