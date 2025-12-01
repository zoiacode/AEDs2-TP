//package TP7.ArvoreBinariaJava;

import java.io.*;
import java.text.*;
import java.util.*;

class Game {
	int id;
	String name;
	String releaseDate;
	int estimatedOwners;
	float price;
	ArrayList<String> supportedLanguages;
	int metacriticScore;
	float userScore;
	int achievements;
	ArrayList<String> publishers;
	ArrayList<String> developers;
	ArrayList<String> categories;
	ArrayList<String> genres;
	ArrayList<String> tags;

	public Game() {
		this.supportedLanguages = new ArrayList<>();
		this.publishers = new ArrayList<>();
		this.developers = new ArrayList<>();
		this.categories = new ArrayList<>();
		this.genres = new ArrayList<>();
		this.tags = new ArrayList<>();
	}

	public static Game fromCSVLine(String line) {
		Game g = new Game();
		String[] fields = splitCSV(line);
		if (fields.length < 14)
			return null;

		g.id = parseIntOrDefault(fields[0], 0);
		g.name = cleanQuotes(fields[1]);
		g.releaseDate = normalizeDate(fields[2]);
		g.estimatedOwners = parseOwners(fields[3]);
		g.price = parsePrice(fields[4]);
		g.supportedLanguages = parseListBracketed(fields[5]);
		g.metacriticScore = parseIntOrDefault(fields[6], -1);
		g.userScore = parseUserScore(fields[7]);
		g.achievements = parseIntOrDefault(fields[8], 0);

		g.publishers = parseListField(fields[9]);
		g.developers = parseListField(fields[10]);
		g.categories = parseListBracketed(fields[11]);
		g.genres = parseListBracketed(fields[12]);
		g.tags = parseListBracketed(fields[13]);

		return g.id != 0 ? g : null;
	}

	private static String cleanQuotes(String s) {
		if (s == null)
			return "";
		s = s.trim();
		if (s.length() >= 2 && ((s.startsWith("\"") && s.endsWith("\"")) || (s.startsWith("'") && s.endsWith("'")))) {
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
		for (char c : s.toCharArray())
			if (Character.isDigit(c))
				digits.append(c);
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
				Date date = fmt.parse(s);
				SimpleDateFormat out = new SimpleDateFormat("dd/MM/yyyy");
				return out.format(date);
			} catch (Exception ignored) {
			}
		}
		return "01/01/0001";
	}

	private static ArrayList<String> parseListBracketed(String s) {
		ArrayList<String> list = new ArrayList<>();
		if (s == null || s.isEmpty())
			return list;

		int start = s.indexOf('[');
		int end = s.lastIndexOf(']');
		String inner = (start >= 0 && end > start) ? s.substring(start + 1, end) : s;

		StringBuilder current = new StringBuilder();
		boolean inQuotes = false;

		for (char c : inner.toCharArray()) {
			if (c == '"' || c == '\'') {
				inQuotes = !inQuotes;
			} else if (c == ',' && !inQuotes) {
				String item = current.toString().trim().replace("\"", "").replace("'", "");
				if (!item.isEmpty())
					list.add(item);
				current.setLength(0);
			} else {
				current.append(c);
			}
		}
		String item = current.toString().trim().replace("\"", "").replace("'", "");
		if (!item.isEmpty())
			list.add(item);

		return list;
	}

	private static ArrayList<String> parseListField(String s) {
		if (s.contains("["))
			return parseListBracketed(s);
		ArrayList<String> list = new ArrayList<>();
		for (String part : s.split(",")) {
			String item = cleanQuotes(part.trim());
			if (!item.isEmpty())
				list.add(item);
		}
		return list;
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

	public void print() {
		System.out.printf(
				"=> %d ## %s ## %s ## %d ## %.2f ## %s ## %d ## %.1f ## %d ## %s ## %s ## %s ## %s ## %s ##%n",
				id, name, releaseDate, estimatedOwners, price,
				formatList(supportedLanguages), metacriticScore, userScore, achievements,
				formatList(publishers), formatList(developers), formatList(categories),
				formatList(genres), formatList(tags));
	}

	private String formatList(ArrayList<String> list) {
		if (list.isEmpty())
			return "[]";
		return "[" + String.join(", ", list) + "]";
	}
}

class No {
	Game elemento;
	No esq, dir;

	public No(Game elemento) {
		this.elemento = elemento;
		this.esq = this.dir = null;
	}
}

class ArvoreBinaria {
	private No raiz;
	private int comparacoes;

	public ArvoreBinaria() {
		raiz = null;
		comparacoes = 0;
	}

	public void inserir(Game game) {
		raiz = inserir(game, raiz);
	}

	private No inserir(Game game, No i) {
		if (i == null) {
			i = new No(game);
		} else if (game.name.compareTo(i.elemento.name) < 0) {
			i.esq = inserir(game, i.esq);
		} else if (game.name.compareTo(i.elemento.name) > 0) {
			i.dir = inserir(game, i.dir);
		}
		return i;
	}

	public boolean pesquisarComCaminho(String name) {
		comparacoes = 0;
		System.out.print(name + ": =>raiz");
		boolean encontrado = pesquisar(name, raiz);
		System.out.println(encontrado ? " SIM" : " NAO");
		return encontrado;
	}

	private boolean pesquisar(String name, No i) {
		if (i == null) {
			return false;
		}
		comparacoes++;
		if (name.equals(i.elemento.name)) {
			return true;
		} else if (name.compareTo(i.elemento.name) < 0) {
			System.out.print(" esq");
			return pesquisar(name, i.esq);
		} else {
			System.out.print(" dir");
			return pesquisar(name, i.dir);
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

		ArvoreBinaria arvore = new ArvoreBinaria();
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
		String nomeArquivoLog = matricula + "_arvoreBinaria.txt";

		try (PrintWriter pw = new PrintWriter(new FileWriter(nomeArquivoLog))) {
			pw.printf("%s\t%d\t%d%n", matricula, totalComparacoes, tempoExecucao);
		} catch (IOException e) {
			System.err.println("Erro ao escrever log: " + e.getMessage());
		}

		sc.close();
	}
}
