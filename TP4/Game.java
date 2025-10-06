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
    private int metacriticScore; // vazio -> -1
    private float userScore; // vazio ou tbd -> -1.0
    private int achievements; // vazio -> 0
    private List<String> publishers;
    private List<String> developers;
    private List<String> categories;
    private List<String> genres;
    private List<String> tags;

    public int getId() {
        return id;
    }

    // ===== Helpers de parsing =====
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
        s = (s == null) ? "" : s.trim();
        if (s.isEmpty())
            return out;
        int l = s.indexOf('['), r = s.lastIndexOf(']');
        String inner = (l != -1 && r != -1 && r > l) ? s.substring(l + 1, r) : s;
        String[] parts = inner.split(",");
        LinkedHashSet<String> uniq = new LinkedHashSet<>();
        for (String p : parts) {
            String item = cleanQuotes(p);
            if (!item.isEmpty())
                uniq.add(item);
        }
        out.addAll(uniq);
        return out;
    }

    private static List<String> parseListComma(String s) {
        if (s == null)
            return new ArrayList<>();
        s = s.trim();
        if (s.isEmpty())
            return new ArrayList<>();
        if (s.contains("[") && s.contains("]"))
            return parseListBracketed(s);
        String[] parts = s.split(",");
        LinkedHashSet<String> uniq = new LinkedHashSet<>();
        for (String p : parts) {
            String item = cleanQuotes(p);
            if (!item.isEmpty())
                uniq.add(item);
        }
        return new ArrayList<>(uniq);
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

    // ===== CSV: vírgula como separador, respeitando aspas e colchetes =====
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
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String header = br.readLine(); // cabeçalho (vírgula)
            if (header == null)
                return map;

            // índices fixos com base no seu header
            // AppID,Name,Release date,Estimated owners,Price,Supported languages,Metacritic
            // score,User score,Achievements,Publishers,Developers,Categories,Genres,Tags
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
                g.publishers = parseListComma(f[9]);
                g.developers = parseListComma(f[10]);
                g.categories = parseListComma(f[11]); // no seu trecho, vem como "Single-player,Steam Achievements,..."
                                                      // entre aspas
                g.genres = parseListComma(f[12]); // idem
                g.tags = parseListComma(f[13]); // idem

                if (g.id != 0)
                    map.put(g.id, g);
            }
        }
        return map;
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
                    System.out.println(g.toJudgeLine());
                }
            } catch (NumberFormatException ignore) {
            }
        }
        sc.close();
    }
}