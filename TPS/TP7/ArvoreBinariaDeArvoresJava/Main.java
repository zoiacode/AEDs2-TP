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

class ArvoreSecundaria {
	NoSeg raiz;

	ArvoreSecundaria() {
		raiz = null;
	}

	void inserir(String nome) {
		raiz = inserir(nome, raiz);
	}

	private NoSeg inserir(String nome, NoSeg i) {
		if (i == null) {
			i = new NoSeg(nome);
		} else if (nome.compareTo(i.chave) < 0) {
			i.esq = inserir(nome, i.esq);
		} else if (nome.compareTo(i.chave) > 0) {
			i.dir = inserir(nome, i.dir);
		}
		return i;
	}

	boolean pesquisar(String nome, StringBuilder caminho) {
		return pesquisar(nome, raiz, caminho);
	}

	private boolean pesquisar(String nome, NoSeg i, StringBuilder caminho) {
		if (i == null) {
			return false;
		}
		if (nome.equals(i.chave)) {
			return true;
		} else if (nome.compareTo(i.chave) < 0) {
			caminho.append(" esq");
			return pesquisar(nome, i.esq, caminho);
		} else {
			caminho.append(" dir");
			return pesquisar(nome, i.dir, caminho);
		}
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
		if (i == null)
			return null;
		if (chave == i.chave)
			return i;
		if (chave < i.chave)
			return buscarNoPrim(chave, i.esq);
		else
			return buscarNoPrim(chave, i.dir);
	}

	void inserirNome(int chavePrim, String nome) {
		NoPrim noPrim = buscarNoPrim(chavePrim);
		if (noPrim == null)
			return;
		if (noPrim.raizSeg == null) {
			noPrim.raizSeg = new NoSeg(nome);
		} else {
			NoSeg raizSeg = noPrim.raizSeg;
			noPrim.raizSeg = inserirNomeSeg(nome, raizSeg);
		}
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

	boolean pesquisarComCaminho(String nome, Game jogo, StringBuilder saida) {
		saida.append("=> ").append(nome).append(" => raiz");
		if (jogo == null) {
			boolean[] achou = { false };
			pesquisarGeral(nome, raiz, saida, achou);
			if (achou[0]) {
				saida.append(" SIM");
			} else {
				saida.append(" NAO");
			}
			return achou[0];
		} else {
			int k = jogo.estimatedOwners % 15;
			return pesquisarPorChavePrimariaEName(k, nome, raiz, saida);
		}
	}

	private boolean pesquisarPorChavePrimariaEName(int chave, String nome, NoPrim i, StringBuilder saida) {
		if (i == null) {
			saida.append("  NAO");
			return false;
		}

		if (chave == i.chave) {
			ArvoreSecundaria arvSeg = new ArvoreSecundaria();
			arvSeg.raiz = i.raizSeg;
			StringBuilder caminhoSeg = new StringBuilder();
			boolean achou = arvSeg.pesquisar(nome, caminhoSeg);
			saida.append(caminhoSeg);
			saida.append(achou ? "  SIM" : "  NAO");
			return achou;
		} else if (chave < i.chave) {
			saida.append("  ESQ");
			return pesquisarPorChavePrimariaEName(chave, nome, i.esq, saida);
		} else {
			saida.append("  DIR");
			return pesquisarPorChavePrimariaEName(chave, nome, i.dir, saida);
		}
	}

	private void pesquisarGeral(String nome, NoPrim i, StringBuilder saida, boolean[] achou) {
		if (i == null || achou[0])
			return;

		ArvoreSecundaria arvSeg = new ArvoreSecundaria();
		arvSeg.raiz = i.raizSeg;
		StringBuilder caminhoSeg = new StringBuilder();
		boolean achouAqui = arvSeg.pesquisar(nome, caminhoSeg);
		saida.append(caminhoSeg);

		if (achouAqui) {
			achou[0] = true;
			return;
		}

		if (i.esq != null && !achou[0]) {
			saida.append("  ESQ");
			pesquisarGeral(nome, i.esq, saida, achou);
			if (achou[0])
				return;
		}

		if (i.dir != null && !achou[0]) {
			saida.append("  DIR");
			pesquisarGeral(nome, i.dir, saida, achou);
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
		int[] chaves = { 7, 3, 11, 1, 5, 9, 13, 0, 2, 4, 6, 8, 10, 12, 14 };
		for (int c : chaves) {
			arvore.inserirChavePrimaria(c);
		}

		Scanner sc = new Scanner(System.in);
		HashMap<String, Game> gamesByName = new HashMap<>();

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
					gamesByName.put(g.name, g);
					int k = g.estimatedOwners % 15;
					arvore.inserirNome(k, g.name);
				}
			} catch (NumberFormatException ignored) {
			}
		}

		while (sc.hasNextLine()) {
			String nome = sc.nextLine().trim();
			if (nome.equals("FIM"))
				break;
			if (nome.isEmpty())
				continue;

			Game jogo = gamesByName.get(nome);
			StringBuilder saida = new StringBuilder();
			arvore.pesquisarComCaminho(nome, jogo, saida);
			System.out.println(saida.toString());
		}

		sc.close();
	}
}