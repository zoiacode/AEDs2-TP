import java.util.Scanner;

public class alegebraBooleana {
    
    private static int pos;
    private static String expression;
    private static int[] values;
    
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        
        while (sc.hasNextLine()) {
            String line = sc.nextLine().trim();
            if (line.isEmpty()) break;
            
            String[] parts = line.split(" ");
            int n = Integer.parseInt(parts[0]);
            
            if (n == 0) break;
            
            values = new int[n];
            for (int i = 0; i < n; i++) {
                values[i] = Integer.parseInt(parts[i + 1]);
            }
            
            StringBuilder sb = new StringBuilder();
            for (int i = n + 1; i < parts.length; i++) {
                sb.append(parts[i]);
                if (i < parts.length - 1) sb.append(" ");
            }
            expression = sb.toString();
            
            pos = 0;
            int result = evaluate();
            System.out.println(result == 1 ? "1" : "0");
        }
        sc.close();
    }
    
    private static int evaluate() {
        skipSpaces();
        
        if (pos < expression.length() && Character.isDigit(expression.charAt(pos))) {
            return expression.charAt(pos++) - '0';
        }
        
        if (pos < expression.length() && Character.isUpperCase(expression.charAt(pos))) {
            int varIndex = expression.charAt(pos++) - 'A';
            return values[varIndex];
        }
        
        if (expression.startsWith("and", pos)) {
            pos += 3;
            skipSpaces();
            expect('(');
            
            int result = 1;
            while (pos < expression.length() && expression.charAt(pos) != ')') {
                skipSpaces();
                if (expression.charAt(pos) == ',') {
                    pos++;
                    continue;
                }
                if (expression.charAt(pos) == ')') break;
                
                int value = evaluate();
                result = result & value;
                skipSpaces();
            }
            expect(')');
            return result;
        }
        
        if (expression.startsWith("or", pos)) {
            pos += 2;
            skipSpaces();
            expect('(');
            
            int result = 0;
            while (pos < expression.length() && expression.charAt(pos) != ')') {
                skipSpaces();
                if (expression.charAt(pos) == ',') {
                    pos++;
                    continue;
                }
                if (expression.charAt(pos) == ')') break;
                
                int value = evaluate();
                result = result | value;
                skipSpaces();
            }
            expect(')');
            return result;
        }
        
        if (expression.startsWith("not", pos)) {
            pos += 3;
            skipSpaces();
            expect('(');
            int value = evaluate();
            skipSpaces();
            expect(')');
            return value == 0 ? 1 : 0;
        }
        
        return 0;
    }
    
    private static void skipSpaces() {
        while (pos < expression.length() && 
               (expression.charAt(pos) == ' ' || expression.charAt(pos) == '\t')) {
            pos++;
        }
    }
    
    private static void expect(char c) {
        skipSpaces();
        if (pos < expression.length() && expression.charAt(pos) == c) {
            pos++;
        }
    }
}