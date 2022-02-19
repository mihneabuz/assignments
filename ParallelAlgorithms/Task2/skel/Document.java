import java.nio.file.Files;
import java.nio.file.Paths;

class Document {
    public static String delimiterString = " ;:/?˜\\.,><‘[]{}()!@#$%ˆ&-+’=*”|\t\r\n";
    public static String delimiterRegex = "[ ;:\\/?˜\\.,><‘\\[\\]{}()!@#$%ˆ&\\-+'=*”|\t\r\n]+";

    private String name;
    private int fragmentSize;
    private int fragmentCount;
    private String content;
    private Rank rank;

    public Document(String path, int fragmentSize) {
        try {
            this.content = new String(Files.readAllBytes(Paths.get(path))).trim();
        } catch (Exception e) {
            System.out.println("CANNOT READ FILE: " + e.toString());
            content = new String();
        }

        this.name = Paths.get(path).getFileName().toString();
        this.fragmentSize = fragmentSize;
        this.fragmentCount = content.length() / fragmentSize + (content.length() % fragmentSize == 0 ? 0 : 1);
        this.rank = new Rank();
    }

    public String getFragment(int i) {
        if (i >= fragmentCount)
            return new String();
        
        int start = i * fragmentSize;
        while (start > 0 && start < content.length() && !isDelimiter(content.charAt(start - 1)))
            start++;

        int end = i == fragmentCount - 1 ? content.length() : (i + 1) * fragmentSize;
        while (end < content.length() && !isDelimiter(content.charAt(end - 1)))
            end++;

        return content.substring(start, end);
    }

    public String getContent() {
        return content;
    }

    public int getFragmentCount() {
        return fragmentCount;
    }

    public String getName() {
        return name;
    }

    public Rank getRank() {
        return rank;
    }

    public void setRank(Rank rank) {
        this.rank = rank;
    }

    private static Boolean isDelimiter(char c) {
        return delimiterString.indexOf(c) != -1;
    }

    public void debug() {
        System.out.println("# " + content + " :: " + fragmentSize + " :: " + fragmentCount);
        for (int i = 0; i < fragmentCount; i++)
            System.out.println("  frag: " + getFragment(i));
    }
}
