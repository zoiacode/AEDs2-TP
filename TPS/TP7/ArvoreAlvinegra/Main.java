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
		if (fields.length < 14)
			return null;
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
		if (s == null)
			return "";
		s = s.trim();
		if (s.length() >= 2 && ((s.startsWith("\"") && s.endsWith("\"")) ||
				(s.startsWith("'") && s.endsWith("'")))) {
			s = s.substring(1, s.length() - 1);
		}
		return s;
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

	private static int parseOwners(String s) {
		s = cleanQuotes(s);
		StringBuilder digits = new StringBuilder();
		for (char c : s.toCharArray()) {
			if (Character.isDigit(c))
				digits.append(c);
		}
		if (digits.length() == 0)
			return 0;
		try {
			return Integer.parseInt(digits.toString());
		} catch (Exception e) {
			return 0;
		}
	}

	private static float parsePrice(String s) {
		s = cleanQuotes(s);
		if (s.isEmpty() || s.equalsIgnoreCase("Free to Play"))
			return 0.0f;
		s = s.replace("$", "").replace(",", "");
		try {
			return Float.parseFloat(s);
		} catch (Exception e) {
			return 0.0f;
		}
	}

	private static float parseUserScore(String s) {
		s = cleanQuotes(s);
		if (s.isEmpty() || s.equalsIgnoreCase("tbd"))
			return -1.0f;
		try {
			return Float.parseFloat(s);
		} catch (Exception e) {
			return -1.0f;
		}
	}

	private static String normalizeDate(String s) {
		s = cleanQuotes(s);
		if (s.isEmpty())
			return "01/01/0001";
		if (s.matches("\\d{4}"))
			return "01/01/" + s;
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
			} catch (Exception ignored) {
			}
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

class No {
	Game elemento;
	No esq, dir;
	boolean cor;

	public No(Game elemento) {
		this.elemento = elemento;
		this.esq = this.dir = null;
		this.cor = true;
	}
}

class ArvoreAlvinegra {
	private No raiz;
	private int comparacoes;

	private static final boolean VERMELHO = true;
	private static final boolean PRETO = false;

	public ArvoreAlvinegra() {
		raiz = null;
		comparacoes = 0;
	}

	private boolean isRed(No x) {
		if (x == null)
			return false;
		return x.cor == VERMELHO;
	}

	private No rotateLeft(No h) {
		No x = h.dir;
		h.dir = x.esq;
		x.esq = h;
		x.cor = h.cor;
		h.cor = VERMELHO;
		return x;
	}

	private No rotateRight(No h) {
		No x = h.esq;
		h.esq = x.dir;
		x.dir = h;
		x.cor = h.cor;
		h.cor = VERMELHO;
		return x;
	}

	private void flipColors(No h) {
		h.cor = !h.cor;
		if (h.esq != null)
			h.esq.cor = !h.esq.cor;
		if (h.dir != null)
			h.dir.cor = !h.dir.cor;
	}

	public void inserir(Game game) {
		raiz = inserir(game, raiz);
		raiz.cor = PRETO;
	}

	private No inserir(Game game, No h) {
		if (h == null) {
			return new No(game);
		}

		int cmp = game.name.compareTo(h.elemento.name);
		if (cmp < 0) {
			h.esq = inserir(game, h.esq);
		} else if (cmp > 0) {
			h.dir = inserir(game, h.dir);
		} else {
			h.elemento = game;
		}

		if (isRed(h.dir) && !isRed(h.esq)) {
			h = rotateLeft(h);
		}

		if (isRed(h.esq) && isRed(h.esq.esq)) {
			h = rotateRight(h);
		}

		if (isRed(h.esq) && isRed(h.dir)) {
			flipColors(h);
		}

		return h;
	}

	public boolean pesquisarComCaminho(String name) {
		comparacoes = 0;
		System.out.print(name + ": =>raiz");
		boolean encontrado = pesquisar(name, raiz, true);
		System.out.println(encontrado ? " SIM" : " NAO");
		return encontrado;
	}

	private boolean pesquisar(String name, No i, boolean primeiroPasso) {
		if (i == null) {
			return false;
		}
		comparacoes++;

		int cmp = name.compareTo(i.elemento.name);
		if (cmp == 0) {
			return true;
		} else if (cmp < 0) {
			System.out.print(primeiroPasso ? "  esq" : " esq");
			return pesquisar(name, i.esq, false);
		} else {
			System.out.print(primeiroPasso ? "  dir" : " dir");
			return pesquisar(name, i.dir, false);
		}
	}

	public int getComparacoes() {
		return comparacoes;
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

		ArvoreAlvinegra arvore = new ArvoreAlvinegra();
		Scanner sc = new Scanner(System.in);

		while (sc.hasNextLine()) {
			String input = sc.nextLine().trim();
			if (input.equals("FIM"))
				break;
			if (input.isEmpty())
				continue;

			try {
				int id = Integer.parseInt(input);
				Game g = allGames.get(id);
				if (g != null) {
					arvore.inserir(g);
				}
			} catch (NumberFormatException ignored) {
			}
		}

		long startTime = System.nanoTime();
		long totalComparacoes = 0;

		while (sc.hasNextLine()) {
			String input = sc.nextLine().trim();
			if (input.equals("FIM"))
				break;
			if (input.isEmpty())
				continue;

			arvore.pesquisarComCaminho(input);
			totalComparacoes += arvore.getComparacoes();
		}

		long endTime = System.nanoTime();
		long tempoExecucao = endTime - startTime;

		String matricula = "840005";
		String nomeArquivoLog = matricula + "_avinegra.txt";

		try (PrintWriter pw = new PrintWriter(new FileWriter(nomeArquivoLog))) {
			pw.printf("%s\t%d\t%d%n", matricula, totalComparacoes, tempoExecucao);
		} catch (IOException e) {
			System.err.println("Erro ao escrever log: " + e.getMessage());
		}

		sc.close();
	}
}