import java.util.*;

public class cesar {
    

    public static boolean Stop(String palavra)
    {
        boolean stop = false;
        if (palavra.length() == 3 && palavra.charAt(0) == 'F' && palavra.charAt(1) == 'I' && palavra.charAt(2) == 'M')
        {
            stop = true;
        }

        return stop;
    }

    public static void Code(String palavra)
    {
        String palavraCode = "";
        int len = palavra.length();

        for (int i = 0; i < len; i++)
        {
            char a = palavra.charAt(i);

            int t = (int) a + 3;

            char b = (char) t;

            //if(t > 127)
            //{
            //    palavraCode += '�';
            //}
            //else{
                palavraCode += b;
            //}

            
        }

        //System.out.println(palavraCode);
        MyIO.println(palavraCode);
    }

    public static void main(String[] args) {

        
        Scanner sc = new Scanner(System.in);
        //String palavra = sc.nextLine();
        String palavra = MyIO.readLine();
                
        while(!Stop(palavra))
        {
            Code(palavra);

            //palavra = sc.nextLine();
            palavra = MyIO.readLine();
        }
        sc.close();
    }
}
