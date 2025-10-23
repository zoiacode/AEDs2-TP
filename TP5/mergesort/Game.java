package TP5.mergesort;

import java.io.*;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

public class Game {
    private int id;
    private String name;
    private String releaseDate; // dd/MM/yyyy
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

    public float getPrice() {
        return price;
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
        String[] inFormats = {
                "MMM d, yyyy", "MMM dd, yyyy",
                "d MMM, yyyy", "dd MMM, yyyy",
                "MMM yyyy",
                "yyyy"
        };
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
            } else {
                cur.append(c);
            }
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

        if (s.contains("[") && s.contains("]")) {
            return parseListBracketed(s);
        }

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

    // mergesort
    private static long comparacoes = 0;
    private static long movimentacoes = 0;

    private static int compareGames(Game g1, Game g2) {
        comparacoes++;

        if (g1.price < g2.price) {
            return -1;
        } else if (g1.price > g2.price) {
            return 1;
        } else {
            return Integer.compare(g1.id, g2.id);
        }
    }

    public static void mergesort(Game[] array) {
        comparacoes = 0;
        movimentacoes = 0;
        mergesort(array, 0, array.length - 1);
    }

    private static void mergesort(Game[] array, int left, int right) {
        if (left < right) {
            int mid = (left + right) / 2;
            mergesort(array, left, mid);
            mergesort(array, mid + 1, right);
            merge(array, left, mid, right);
        }
    }

    private static void merge(Game[] array, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        Game[] L = new Game[n1];
        Game[] R = new Game[n2];

        for (int i = 0; i < n1; i++) {
            L[i] = array[left + i];
            movimentacoes++;
        }
        for (int j = 0; j < n2; j++) {
            R[j] = array[mid + 1 + j];
            movimentacoes++;
        }

        int i = 0, j = 0;
        int k = left;
        while (i < n1 && j < n2) {
            if (compareGames(L[i], R[j]) <= 0) {
                array[k] = L[i];
                movimentacoes++;
                i++;
            } else {
                array[k] = R[j];
                movimentacoes++;
                j++;
            }
            k++;
        }

        while (i < n1) {
            array[k] = L[i];
            movimentacoes++;
            i++;
            k++;
        }

        while (j < n2) {
            array[k] = R[j];
            movimentacoes++;
            j++;
            k++;
        }
    }

    // ===== Main =====
    public static void main(String[] args) {
        Map<Integer, Game> byId;
        try {
            byId = loadAllGames("/tmp/games.csv");
        } catch (IOException e) {
            System.err.println("Falha ao abrir games.csv: " + e.getMessage());
            return;
        }

        Scanner sc = new Scanner(System.in);
        List<Game> selectedGames = new ArrayList<>();

        while (sc.hasNextLine()) {
            String s = sc.nextLine().trim();
            if (s.equals("FIM"))
                break;
            if (s.isEmpty())
                continue;
            try {
                int id = Integer.parseInt(s);
                Game g = byId.get(id);
                if (g != null) {
                    selectedGames.add(g);
                }
            } catch (NumberFormatException ignore) {
            }
        }
        sc.close();

        Game[] games = selectedGames.toArray(new Game[0]);

        if (games.length == 0) {
            return;
        }

        long startTime = System.nanoTime();
        mergesort(games);
        long endTime = System.nanoTime();
        long tempoExecucao = endTime - startTime;

        for (Game g : games) {
            // System.out.println(g.toJudgeLine());
        }

        MyIO.println("| 5 preços mais caros |");
        for (int i = games.length - 1; i >= Math.max(0, games.length - 5); i--) {
            MyIO.println(games[i].toJudgeLine());
        }

        MyIO.println("");

        MyIO.println("| 5 preços mais baratos |");
        for (int i = 0; i < Math.min(5, games.length); i++) {
            MyIO.println(games[i].toJudgeLine());
        }

        String matricula = "840005";
        try (PrintWriter writer = new PrintWriter(new FileWriter(matricula + "_mergesort.txt"))) {
            writer.println(matricula + "\t" + comparacoes + "\t" + movimentacoes + "\t" + tempoExecucao);
        } catch (IOException e) {
            System.err.println("Erro ao criar arquivo de log: " + e.getMessage());
        }
    }
}