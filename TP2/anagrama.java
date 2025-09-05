//import java.util.Scanner;

public class anagrama
{

    public static boolean stop(String str1)
    {
        boolean answer = false;

        if (str1. length() == 3 && str1.charAt(0) == 'F' && str1.charAt(1) == 'I' && str1.charAt(2) == 'M') 
        {
            answer = true;
        }
        return answer;
    }

    public static boolean isAnagrama(String str1, String str2)
    {
        str1 = str1.toLowerCase();
        str2 = str2.toLowerCase();

        int[] counter = new int[256];

        if (str1.length() != str2.length())
        {
            return false;
        }

        for (int i = 0; i < str1.length(); i++)
        {
            counter[str1.charAt(i)]++;
        }

        for(int j = 0; j < str2.length(); j++)
        {
            counter[str2.charAt(j)]--;
        }

        for (int i = 0; i < 256; i++)
        {
            if (counter[i] != 0)
            {
                return false;
            }
        }

        return true;
    }
    public static void main(String[] args)
    {
        //Scanner scanf = new Scanner(System.in);

        String linha = MyIO.readLine();

        while (!stop(linha))
        {
            String[] palavras = linha.split(" - ");

            if (isAnagrama(palavras[0], palavras[1]))
            {
                MyIO.println("SIM");
            }
            else
            {
                MyIO.println("NÃO");
            }
            linha = MyIO.readLine();
            //linha = scanf.nextLine();
        }
        //scanf.close(); 
    }
}