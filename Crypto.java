import java.util.*;

public class Crypto {
    public static void main(String[] args){
        Scanner input = new Scanner(System.in);
        System.out.println("Please input the texts you want to encrypt: ");
        String plaintext = input.nextLine();
        System.out.println("Please input the key: ");
        int key = input.nextInt();
        System.out.println("Please input the code group size: ");
        int group = input.nextInt();

        String cyphertext = encryptString(plaintext, key, group);
        System.out.println("Cyphertext : " + cyphertext);
        System.out.println("Plaintext : " + decryptString(cyphertext, key));
    }

    public static String normalizeText(String str){
        return str.replaceAll("\\s", "").replaceAll("[^a-zA-Z]", "").toUpperCase();

    }

    public static String obify(String str){
        String new_str = "";
        for(int i = 0, n = str.length(); i < n; i++){
            char c = str.charAt(i);
            if(c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'Y'){
                new_str += "OB";
            }
            new_str += c;
        }

        return new_str;
    }

    public static String caesarify(String str, int key){
        String out = "";
        for(int i = 0, n = str.length(); i < n; i++){
            int shift = ((int)str.charAt(i) - 65 + key) % 26;
            if(shift < 0)
                shift = 26 + shift;
            out += (char)(shift + 65);
        }
        return out;
    }

    public static String groupify(String s, int step){
        String g_str = "";
        for(int i = 1, n = s.length(); i <= n; i++){
            g_str += s.charAt(i - 1);

            if(i % step == 0)
                g_str += " ";
            else if(i == n)
                g_str += "x";

        }
        return g_str;
    }

    public static String encryptString(String s, int key, int group){
        return groupify(caesarify(obify(normalizeText(s)), key), group);
    }

    public static String decryptString (String s ,int key){
        String plaintext = s;
        if(s.endsWith("x"))
            plaintext = s.substring(0, s.length() - 1);
        return caesarify(plaintext.replaceAll("\\s", ""), -key).replaceAll("OB", "");
    }
}
