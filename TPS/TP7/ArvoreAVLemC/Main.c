import java.io.*;
import java.text.*;
import java.util.*;

class Game {
    int id;
    String name;
    String releaseDate;
    int estimatedOwners;
    float price;
    int metacriticScore;
    float userScore;
    int achievements;

    public static Game fromCSVLine(String line) {
        String[] fields = splitCSV(line);
        if (fields.length < 14) return null;
        Game g = new Game();
        g.id = parseIntOrDefault(fields[0], 0);
        g.name = cleanQuotes(fields[1]);
        g.releaseDate = normalizeDate(fields[2]);
        g.estimatedOwners = parseOwners(fields[3]);
        g.price = parsePrice(fields[4]);
        g.metacriticScore = parseIntOrDefault(fields[6], -1);
        g.userScore = parseUserScore(fields[7]);
        g.achievements = parseIntOrDefault(fields[8], 0);
        return g.id != 0 ? g : null;
    }

    private static String cleanQuotes(String s) {
        if (s == null) return "";
        s = s.trim();
        if (s.length() >= 2 && ((s.startsWith("\"") && s.endsWith("\"")) ||
                                (s.startsWith("'") && s.endsWith("'")))) {
            s = s.substring(1, s.length() - 1);
        }
        return s;
    }

    private static int parseIntOrDefault(String s, int def) {
        s = cleanQuotes(s);
        if (s.isEmpty()) return def;
        try { return Integer.parseInt(s); } catch (Exception e) { return def; }
    }

    private static int parseOwners(String s) {
        s = cleanQuotes(s);
        StringBuilder digits = new StringBuilder();
        for (char c : s.toCharArray()) {
            if (Character.isDigit(c)) digits.append(c);
        }
        if (digits.length() == 0) return 0;
        try { return Integer.parseInt(digits.toString()); } catch (Exception e) { return 0; }
    }

    private static float parsePrice(String s) {
        s = cleanQuotes(s);
        if (s.isEmpty() || s.equalsIgnoreCase("Free to Play")) return 0.0f;
        s = s.replace("$", "").replace(",", "");
        try { return Float.parseFloat(s); } catch (Exception e) { return 0.0f; }
    }

    private static float parseUserScore(String s) {
        s = cleanQuotes(s);
        if (s.isEmpty() || s.equalsIgnoreCase("tbd")) return -1.0f;
        try { return Float.parseFloat(s); } catch (Exception e) { return -1.0f; }
    }

    private static String normalizeDate(String s) {
        s = cleanQuotes(s);
        if (s.isEmpty()) return "01/01/0001";
        if (s.matches("\\d{4}")) return "01/01/" + s;
        SimpleDateFormat[] formats = {
            new SimpleDateFormat("MMM dd, yyyy", Locale.ENGLISH),
            new SimpleDateFormat("MMMM dd, yyyy", Locale.ENGLISH),
            new SimpleDateFormat("dd MMM, yyyy", Locale.ENGLISH),
            new SimpleDateFormat("dd MMMM, yyyy", Locale.ENGLISH),
            new SimpleDateFormat("MMM yyyy", Locale.ENGLISH),
            new SimpleDateFormat("MMMM yyyy", Locale.ENGLISH)
        };
        for (SimpleDateFormat fmt : formats) {
            try {
                Date d = fmt.parse(s);
                return new SimpleDateFormat("dd/MM/yyyy").format(d);
            } catch (Exception ignored) {}
        }
        return "01/01/0001";
    }

    private static String[] splitCSV(String line) {
        ArrayList<String> fields = new ArrayList<>();
        StringBuilder current = new StringBuilder();
        boolean inQuotes = false;
        int bracketLevel = 0;
        for (char c : line.toCharArray()) {
            if (c == '"') {
                inQuotes = !inQuotes;
                current.append(c);
            } else if (c == '[' && !inQuotes) {
                bracketLevel++;
                current.append(c);
            } else if (c == ']' && !inQuotes && bracketLevel > 0) {
                bracketLevel--;
                current.append(c);
            } else if (c == ',' && !inQuotes && bracketLevel == 0) {
                fields.add(current.toString());
                current.setLength(0);
            } else {
                current.append(c);
            }
        }
        fields.add(current.toString());
        return fields.toArray(new String[0]);
    }
}

class NoSeg {
    String chave;
    NoSeg esq, dir;

    NoSeg(String chave) {
        this.chave = chave;
        this.esq = this.dir = null;
    }
}

class NoPrim {
    int chave;
    NoPrim esq, dir;
    NoSeg raizSeg;

    NoPrim(int chave) {
        this.chave = chave;
        this.esq = this.dir = null;
        this.raizSeg = null;
    }
}

class ArvorePrimaria {
    NoPrim raiz;

    ArvorePrimaria() {
        raiz = null;
    }

    void inserirChavePrimaria(int chave) {
        raiz = inserirChavePrimaria(chave, raiz);
    }

    private NoPrim inserirChavePrimaria(int chave, NoPrim i) {
        if (i == null) {
            i = new NoPrim(chave);
        } else if (chave < i.chave) {
            i.esq = inserirChavePrimaria(chave, i.esq);
        } else if (chave > i.chave) {
            i.dir = inserirChavePrimaria(chave, i.dir);
        }
        return i;
    }

    NoPrim buscarNoPrim(int chave) {
        return buscarNoPrim(chave, raiz);
    }

    private NoPrim buscarNoPrim(int chave, NoPrim i) {
        if (i == null) return null;
        if (chave == i.chave) return i;
        if (chave < i.chave) return buscarNoPrim(chave, i.esq);
        else return buscarNoPrim(chave, i.dir);
    }

    void inserirNome(int chavePrim, String nome) {
        NoPrim noPrim = buscarNoPrim(chavePrim);
        if (noPrim == null) return;
        noPrim.raizSeg = inserirNomeSeg(nome, noPrim.raizSeg);
    }

    private NoSeg inserirNomeSeg(String nome, NoSeg i) {
        if (i == null) {
            i = new NoSeg(nome);
        } else if (nome.compareTo(i.chave) < 0) {
            i.esq = inserirNomeSeg(nome, i.esq);
        } else if (nome.compareTo(i.chave) > 0) {
            i.dir = inserirNomeSeg(nome, i.dir);
        }
        return i;
    }

    boolean pesquisarComCaminho(String nome, StringBuilder saida) {
        saida.append("=> ").append(nome).append(" => raiz");
        boolean[] achou = {false};
        mostrarPrimeira(nome, raiz, saida, achou);
        saida.append(achou[0] ? " SIM" : " NAO");
        return achou[0];
    }

    private void mostrarPrimeira(String nome, NoPrim i, StringBuilder saida, boolean[] achou) {
        if (i == null || achou[0]) return;

        mostrarSegunda(nome, i.raizSeg, saida, achou);
        if (achou[0]) return;

        // Percorre esquerda
        if (i.esq != null && !achou[0]) {
            saida.append("  ESQ");
            mostrarPrimeira(nome, i.esq, saida, achou);
            if (achou[0]) return;
        }

        // Percorre direita
        if (i.dir != null && !achou[0]) {
            saida.append("  DIR");
            mostrarPrimeira(nome, i.dir, saida, achou);
        }
    }

    private void mostrarSegunda(String nome, NoSeg i, StringBuilder saida, boolean[] achou) {
        if (i == null || achou[0]) return;

        if (nome.equals(i.chave)) {
            achou[0] = true;
            return;
        }

        // Percorre esquerda
        if (i.esq != null && !achou[0]) {
            saida.append(" esq");
            mostrarSegunda(nome, i.esq, saida, achou);
            if (achou[0]) return;
        }

        // Percorre direita
        if (i.dir != null && !achou[0]) {
            saida.append(" dir");
            mostrarSegunda(nome, i.dir, saida, achou);
        }
    }
}

public class Main {
    public static void main(String[] args) {
        String csvPath = "/tmp/games.csv";
        HashMap<Integer, Game> allGames = new HashMap<>();

        try (BufferedReader br = new BufferedReader(new FileReader(csvPath))) {
            br.readLine(); 
            String line;
            while ((line = br.readLine()) != null) {
                Game g = Game.fromCSVLine(line);
                if (g != null) {
                    allGames.put(g.id, g);
                }
            }
        } catch (IOException e) {
            System.err.println("Erro ao ler CSV: " + e.getMessage());
            return;
        }

        ArvorePrimaria arvore = new ArvorePrimaria();
        int[] chaves = {7, 3, 11, 1, 5, 9, 13, 0, 2, 4, 6, 8, 10, 12, 14};
        for (int c : chaves) {
            arvore.inserirChavePrimaria(c);
        }

        Scanner sc = new Scanner(System.in);

        while (sc.hasNextLine()) {
            String input = sc.nextLine().trim();
            if (input.equals("FIM")) break;
            if (input.isEmpty()) continue;

            try {
                int id = Integer.parseInt(input);
                Game g = allGames.get(id);
                if (g != null) {
                    int k = g.estimatedOwners % 15;
                    arvore.inserirNome(k, g.name);
                }
            } catch (NumberFormatException ignored) {}
        }

        while (sc.hasNextLine()) {
            String nome = sc.nextLine().trim();
            if (nome.equals("FIM")) break;
            if (nome.isEmpty()) continue;

            StringBuilder saida = new StringBuilder();
            arvore.pesquisarComCaminho(nome, saida);
            System.out.println(saida.toString());
        }

        sc.close();
    }
}