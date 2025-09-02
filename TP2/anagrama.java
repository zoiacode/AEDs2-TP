import java.util.Scanner;

public class anagrama
{
    public static boolean stop(String input) {
        boolean resp = false;

        if (input.length() == 3 && input.charAt(0) == 'F' && input.charAt(1) == 'I' && input.charAt(2) == 'M') 
        {
            resp = true;
        }

        return resp;
    }
    public static void main(String[] args)
    {
        Scanner scanf = new Scanner(System.in);
        String input = scanf.next();
        String input2 = scanf.next();
        String input3 = scanf.next();

        while(!stop(input))
        {
            if (isAnagram(input, input3))
            {
                System.out.println("SIM");
            }
            else if(!isAnagram(input2, input3))
            {
                System.out.println("NÃO");
            }
        }

        input = scanf.nextLine();
        input2 = scanf.nextLine();
        input3 = scanf.nextLine();


        scanf.close();
    }

    static boolean isAnagram(String str1, String str2) {
        if (str1.length() != str2.length()) {
            return false;
        }

        int[] charCount = new int[256];

        for (int i = 0; i < str1.length(); i++) {
            charCount[str1.charAt(i)]++;
            charCount[str2.charAt(i)]--;
        }

        for (int count : charCount) {
            if (count != 0) {
                return false;
            }
        }

        return true;
    }
}