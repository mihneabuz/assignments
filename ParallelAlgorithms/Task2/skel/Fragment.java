import java.util.HashMap;
import java.util.ArrayList;

class Fragment {
    private String doc;
    private HashMap<Integer, Integer> occurances;
    private int longest;
    private ArrayList<String> longestWords;

    public Fragment(String doc, String frag) {
        this.doc = doc;
        this.occurances = new HashMap<>();
        this.longestWords = new ArrayList<>();
        this.longest = 0;

        for (String word : frag.split(Document.delimiterRegex)) {
            if (!word.isBlank()) {
                int letters = word.length();

                if (letters > longest) {
                    longest = letters;
                    longestWords.clear();
                    longestWords.add(word);
                } else if (letters == longest) {
                    longestWords.add(word);
                }

                if (occurances.containsKey(letters)) {
                    occurances.put(letters, occurances.get(letters) + 1);
                } else {
                    occurances.put(letters, 1);
                }
            }
        }
    }

    public String getDoc() {
        return doc;
    }

    public HashMap<Integer, Integer> getOccurances() {
        return occurances;
    }

    public int getLongest() {
        return longest;
    } 

    public int getLongestCount() {
        return longestWords.size();
    } 

    public String toString() {
        return "Document: [" + doc + "] Map: " + occurances.toString() + " Words: " + longestWords.toString() + "";
    }
}
