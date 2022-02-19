import java.util.HashMap;

class Rank {
    private String name;
    private float value;
    private int longest;
    private int longestCount;
    private HashMap<Integer, Integer> occurances;

    public Rank() {
        this.name = "";
    }

    public Rank(String name, int value, int longest, int longestCount) {
        this.name = name;
        this.value = value;
        this.longest = longest;
        this.longestCount = longestCount;
    }

    public void combine(Fragment frag) {
        if (this.name == "") {
            this.name = frag.getDoc();
            this.longest = frag.getLongest();
            this.longestCount = frag.getLongestCount();
            this.occurances = frag.getOccurances();
        } else {
            HashMap<Integer, Integer> fragMap = frag.getOccurances();
            for (Integer key : fragMap.keySet()) {
                if (occurances.containsKey(key)) {
                    occurances.put(key, occurances.get(key) + fragMap.get(key));
                } else {
                    occurances.put(key, fragMap.get(key));
                }
            }

            if (frag.getLongest() > longest) {
                longest = frag.getLongest();
                longestCount = frag.getLongestCount();
            } else if (frag.getLongest() == longest) {
                longestCount += frag.getLongestCount();
            }
        }
    }

    public void compute() {
        int total = 0;
        value = 0;
        for (Integer key : occurances.keySet()) {
            value += Fibonacci.get(key + 1) * occurances.get(key);
            total += occurances.get(key);
        }
        value /= total;
    }

    public float getValue() {
        return value;
    }

    public String toString() {
        return "[" + name + "] Rank: [" + value + "] Longest: ["  + longest + "] Count: [" + longestCount + "]";
    }

    public String format() {
        return name + "," + String.format("%.2f", value) + "," + longest + "," + longestCount;
    }
}
