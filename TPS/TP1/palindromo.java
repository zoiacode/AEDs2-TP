import java.util.*;

public class palindromo {


    public static boolean isPal(String palavra)
    {
        boolean resp = true;
        int x = palavra.length();
        int z = x - 1;

        
        for (int i = 0; i < x / 2; i++)
        {
            if(palavra.charAt(i) != palavra.charAt(z))
            {
                resp = false;
            }
            z--;
        }
        
        return resp;
    }
    public static void main (String[] args)
    {
      Scanner sc = new Scanner(System.in);

      String palavra = sc.nextLine();

      while(!palavra.equals("FIM"))
      {
        if (isPal(palavra))
        {
            System.out.println("SIM");
        }else
        {
            System.out.println("NAO");    
        }
        palavra = sc.nextLine();
      }
      
      sc.close();
    }
}