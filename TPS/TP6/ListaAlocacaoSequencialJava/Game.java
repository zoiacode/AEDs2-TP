import java.io.*;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

class Lista {
    private Game[] vet;
    private int n;

    public Lista(int capacidade) {
        vet = new Game[capacidade];
        n = 0;
    }

    private void ensureCapacity() {
        if (n >= vet.length) {
            Game[] novo = new Game[vet.length * 2 + 1];
            System.arraycopy(vet, 0, novo, 0, n);
            vet = novo;
        }
    }

    public void inserirInicio(Game game) {
        if (game == null)
            return;
        ensureCapacity();
        for (int i = n; i > 0; i--) {
            vet[i] = vet[i - 1];
        }
        vet[0] = game;
        n++;
    }

    public void inserir(Game game, int posicao) {
        if (game == null)
            return;
        if (posicao < 0 || posicao > n)
            throw new IllegalArgumentException("Posição inválida");
        ensureCapacity();
        for (int i = n; i > posicao; i--) {
            vet[i] = vet[i - 1];
        }
        vet[posicao] = game;
        n++;
    }

    public void inserirFim(Game game) {
        if (game == null)
            return;
        ensureCapacity();
        vet[n++] = game;
    }

    public Game removerInicio() {
        if (n == 0)
            throw new RuntimeException("Lista vazia");
        Game resp = vet[0];
        for (int i = 0; i < n - 1; i++) {
            vet[i] = vet[i + 1];
        }
        vet[--n] = null;
        return resp;
    }

    public Game remover(int posicao) {
        if (n == 0)
            throw new RuntimeException("Lista vazia");
        if (posicao < 0 || posicao >= n)
            throw new IllegalArgumentException("Posição inválida");
        Game resp = vet[posicao];
        for (int i = posicao; i < n - 1; i++) {
            vet[i] = vet[i + 1];
        }
        vet[--n] = null;
        return resp;
    }

    public Game removerFim() {
        if (n == 0)
            throw new RuntimeException("Lista vazia");
        Game resp = vet[--n];
        vet[n] = null;
        return resp;
    }

    public int tamanho() {
        return n;
    }

    public void mostrar() {
        for (int i = 0; i < n; i++) {
            System.out.println("[" + i + "] " + vet[i].toJudgeLine());
        }
    }
}

public class Game {
    private int id;
    private String name;
    private String releaseDate;
    private int estimatedOwners;
    private float price;
    private List<String> supportedLanguages;
    private int metacriticScore;
    private float userScore;
    private int achievements;
    private List<String> publishers;
    private List<String> developers;
    private List<String> categories;
    private List<String> genres;
    private List<String> tags;

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    private static String cleanQuotes(String s) {
        if (s == null)
            return "";
        s = s.trim();
        if ((s.startsWith("\"") && s.endsWith("\"")) || (s.startsWith("'") && s.endsWith("'"))) {
            if (s.length() >= 2)
                s = s.substring(1, s.length() - 1);
        }
        return s.trim();
    }

    private static int parseIntOrDefault(String s, int def) {
        s = cleanQuotes(s);
        if (s.isEmpty())
            return def;
        try {
            return Integer.parseInt(s);
        } catch (Exception e) {
            return def;
        }
    }

    private static float parseFloatOrDefault(String s, float def) {
        s = cleanQuotes(s);
        if (s.isEmpty())
            return def;
        try {
            return Float.parseFloat(s);
        } catch (Exception e) {
            return def;
        }
    }

    private static int parseOwners(String s) {
        s = cleanQuotes(s);
        if (s.isEmpty())
            return 0;
        String digits = s.replaceAll("[^0-9]", "");
        if (digits.isEmpty())
            return 0;
        try {
            return Integer.parseInt(digits);
        } catch (Exception e) {
            return 0;
        }
    }

    private static float parsePrice(String s) {
        s = cleanQuotes(s);
        if (s.isEmpty())
            return 0.0f;
        if (s.equalsIgnoreCase("Free to Play"))
            return 0.0f;
        s = s.replace("$", "").replace(",", "").trim();
        return parseFloatOrDefault(s, 0.0f);
    }

    private static float parseUserScore(String s) {
        s = cleanQuotes(s);
        if (s.isEmpty())
            return -1.0f;
        if (s.equalsIgnoreCase("tbd"))
            return -1.0f;
        return parseFloatOrDefault(s, -1.0f);
    }

    private static String normalizeDate(String s) {
        s = cleanQuotes(s);
        if (s.isEmpty())
            return "01/01/0001";
        String[] inFormats = { "MMM d, yyyy", "MMM dd, yyyy", "d MMM, yyyy", "dd MMM, yyyy", "MMM yyyy", "yyyy" };
        for (String fmt : inFormats) {
            try {
                SimpleDateFormat sdf = new SimpleDateFormat(fmt, Locale.ENGLISH);
                sdf.setLenient(false);
                Date d = sdf.parse(s);
                return new SimpleDateFormat("dd/MM/yyyy").format(d);
            } catch (ParseException ignore) {
            }
        }
        if (s.matches("\\d{4}")) {
            int y = Integer.parseInt(s);
            return String.format("%02d/%02d/%04d", 1, 1, y);
        }
        String digits = s.replaceAll("[^0-9]", " ").trim().replaceAll("\\s+", " ");
        String[] parts = digits.isEmpty() ? new String[0] : digits.split(" ");
        if (parts.length == 3) {
            try {
                int d = Integer.parseInt(parts[0]);
                int m = Integer.parseInt(parts[1]);
                int y = Integer.parseInt(parts[2]);
                return String.format("%02d/%02d/%04d", d, m, y);
            } catch (Exception ignore) {
            }
        } else if (parts.length == 2) {
            try {
                int m = Integer.parseInt(parts[0]);
                int y = Integer.parseInt(parts[1]);
                return String.format("%02d/%02d/%04d", 1, m, y);
            } catch (Exception ignore) {
            }
        }
        return "01/01/0001";
    }

    private static List<String> parseListBracketed(String s) {
        List<String> out = new ArrayList<>();
        if (s == null)
            return out;
        s = s.trim();
        if (s.isEmpty())
            return out;

        int l = s.indexOf('['), r = s.lastIndexOf(']');
        String inner = (l != -1 && r != -1 && r > l) ? s.substring(l + 1, r) : s;

        List<String> parts = new ArrayList<>();
        StringBuilder cur = new StringBuilder();
        boolean inQuotes = false;
        for (int i = 0; i < inner.length(); i++) {
            char c = inner.charAt(i);
            if (c == '"') {
                inQuotes = !inQuotes;
                continue;
            }
            if (c == ',' && !inQuotes) {
                parts.add(cur.toString());
                cur.setLength(0);
            } else
                cur.append(c);
        }
        parts.add(cur.toString());

        LinkedHashSet<String> uniq = new LinkedHashSet<>();
        for (String p : parts) {
            String item = p.trim();
            item = cleanQuotes(item);
            item = item.replace("\"\"", "\"").replace("\\\"", "\"");
            if (item.startsWith("\"") && item.endsWith("\"") && item.length() >= 2) {
                item = item.substring(1, item.length() - 1).trim();
            }
            if (item.startsWith("'") && item.endsWith("'") && item.length() >= 2) {
                item = item.substring(1, item.length() - 1).trim();
            }
            item = item.replace("\"", "");
            if (!item.isEmpty())
                uniq.add(item);
        }
        out.addAll(uniq);
        return out;
    }

    private static List<String> parseListComma(String s) {
        List<String> out = new ArrayList<>();
        if (s == null)
            return out;
        s = s.trim();
        if (s.isEmpty())
            return out;

        if (s.contains("[") && s.contains("]"))
            return parseListBracketed(s);
        String[] parts = s.split(",");
        LinkedHashSet<String> uniq = new LinkedHashSet<>();
        for (String p : parts) {
            String item = p.trim();
            item = cleanQuotes(item);
            item = item.replace("\"\"", "\"").replace("\\\"", "\"");
            if (item.startsWith("\"") && item.endsWith("\"") && item.length() >= 2) {
                item = item.substring(1, item.length() - 1).trim();
            }
            if (item.startsWith("'") && item.endsWith("'") && item.length() >= 2) {
                item = item.substring(1, item.length() - 1).trim();
            }
            item = item.replace("\"", "");
            if (!item.isEmpty())
                uniq.add(item);
        }
        out.addAll(uniq);
        return out;
    }

    private static String formatList(List<String> list) {
        if (list == null || list.isEmpty())
            return "[]";
        return "[" + String.join(", ", list) + "]";
    }

    private static String formatFloatForOutput(float v) {
        String s = Float.toString(v);
        if (s.endsWith(".0")) {
            if (v == 0.0f || v == -1.0f)
                return s;
            return s.substring(0, s.length() - 2);
        }
        s = s.replaceAll("(\\.\\d*?[1-9])0+$", "$1");
        return s;
    }

    public String toJudgeLine() {
        return "=> " + id + " ## " + name + " ## " + releaseDate + " ## " + estimatedOwners + " ## "
                + formatFloatForOutput(price) + " ## " + formatList(supportedLanguages) + " ## "
                + metacriticScore + " ## " + formatFloatForOutput(userScore) + " ## " + achievements + " ## "
                + formatList(publishers) + " ## " + formatList(developers) + " ## " + formatList(categories)
                + " ## " + formatList(genres) + " ## " + formatList(tags) + " ##";
    }

    private static String[] splitCSV(String line) {
        List<String> fields = new ArrayList<>();
        StringBuilder cur = new StringBuilder();
        boolean inQuotes = false;
        int bracketLevel = 0;
        for (int i = 0; i < line.length(); i++) {
            char c = line.charAt(i);
            if (c == '"') {
                inQuotes = !inQuotes;
                cur.append(c);
            } else if (c == '[' && !inQuotes) {
                bracketLevel++;
                cur.append(c);
            } else if (c == ']' && !inQuotes && bracketLevel > 0) {
                bracketLevel--;
                cur.append(c);
            } else if (c == ',' && !inQuotes && bracketLevel == 0) {
                fields.add(cur.toString());
                cur.setLength(0);
            } else {
                cur.append(c);
            }
        }
        fields.add(cur.toString());
        return fields.toArray(new String[0]);
    }

    private static Map<Integer, Game> loadAllGames(String fileName) throws IOException {
        Map<Integer, Game> map = new HashMap<>();
        try (BufferedReader br = new BufferedReader(
                new InputStreamReader(new FileInputStream(fileName), java.nio.charset.StandardCharsets.UTF_8))) {
            String header = br.readLine();
            if (header == null)
                return map;
            String line;
            while ((line = br.readLine()) != null) {
                if (line.isEmpty())
                    continue;
                String[] f = splitCSV(line);
                if (f.length < 14)
                    continue;

                Game g = new Game();
                g.id = parseIntOrDefault(f[0], 0);
                g.name = cleanQuotes(f[1]);
                g.releaseDate = normalizeDate(f[2]);
                g.estimatedOwners = parseOwners(f[3]);
                g.price = parsePrice(f[4]);
                g.supportedLanguages = parseListBracketed(f[5]);
                g.metacriticScore = parseIntOrDefault(f[6], -1);
                g.userScore = parseUserScore(f[7]);
                g.achievements = parseIntOrDefault(f[8], 0);
                g.publishers = (f[9].contains("[") ? parseListBracketed(f[9]) : parseListComma(f[9]));
                g.developers = (f[10].contains("[") ? parseListBracketed(f[10]) : parseListComma(f[10]));
                g.categories = (f[11].contains("[") ? parseListBracketed(f[11]) : parseListComma(f[11]));
                g.genres = (f[12].contains("[") ? parseListBracketed(f[12]) : parseListComma(f[12]));
                g.tags = (f[13].contains("[") ? parseListBracketed(f[13]) : parseListComma(f[13]));

                if (g.id != 0)
                    map.put(g.id, g);
            }
        }
        return map;
    }

    public static void main(String[] args) {
        Map<Integer, Game> byId;
        try {
            byId = loadAllGames("/tmp/games.csv");
        } catch (IOException e) {
            System.err.println("Falha ao abrir games.csv: " + e.getMessage());
            return;
        }

        Scanner sc = new Scanner(System.in);
        Lista lista = new Lista(100);
        int remCount = 1;

        try {
            while (sc.hasNextLine()) {
                String linha = sc.nextLine().trim();
                if (linha.equalsIgnoreCase("FIM"))
                    break;
                if (linha.isEmpty())
                    continue;
                int gameId = Integer.parseInt(linha);
                Game g = byId.get(gameId);
                if (g != null)
                    lista.inserirFim(g);
            }

            if (sc.hasNextLine()) {
                String qStr = sc.nextLine().trim();
                qStr = qStr.replaceAll("[^0-9]", "");
                if (!qStr.isEmpty()) {
                    int numComandos = Integer.parseInt(qStr);
                    for (int i = 0; i < numComandos; i++) {
                        if (!sc.hasNextLine())
                            break;
                        String cmdLine = sc.nextLine().trim();
                        if (cmdLine.isEmpty()) {
                            i--;
                            continue;
                        }

                        String[] partes = cmdLine.split("\\s+");
                        String op = partes[0];

                        switch (op) {
                            case "II": {
                                int id = Integer.parseInt(partes[1]);
                                Game g = byId.get(id);
                                if (g != null)
                                    lista.inserirInicio(g);
                                break;
                            }
                            case "IF": {
                                int id = Integer.parseInt(partes[1]);
                                Game g = byId.get(id);
                                if (g != null)
                                    lista.inserirFim(g);
                                break;
                            }
                            case "I*": {
                                int pos = Integer.parseInt(partes[1]);
                                int id = Integer.parseInt(partes[2]);
                                Game g = byId.get(id);
                                if (g != null)
                                    lista.inserir(g, pos);
                                break;
                            }
                            case "RI": {
                                Game r = lista.removerInicio();
                                System.out.println("(R) " + r.getName());
                                break;
                            }
                            case "RF": {
                                Game r = lista.removerFim();
                                System.out.println("(R) " + r.getName());
                                break;
                            }
                            case "R*": {
                                int pos = Integer.parseInt(partes[1]);
                                Game r = lista.remover(pos);
                                System.out.println("(R) " + r.getName());
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }

            lista.mostrar();

        } catch (Exception e) {
            System.err.println("Erro: " + e.getMessage());
        } finally {
            sc.close();
        }
    }
}