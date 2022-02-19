import java.util.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.PrintWriter;

public class Tema2 {
    public static void main(String[] args) {
        if (args.length < 3) {
            System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            return;
        }

        int processors = Integer.parseInt(args[0]);
        String inFile = args[1];
        String outFile = args[2];

        List<String> lines;
        try {
            lines = Files.readAllLines(Paths.get(inFile));
        } catch (Exception e) {
            System.out.println("Cannot read input file");
            System.out.println(e.toString());
            return;
        }

        int fragmentSize = Integer.parseInt(lines.get(0));
        int documentCount = Integer.parseInt(lines.get(1));

        ArrayList<Document> documents = new ArrayList<>();
        for (int i = 0; i < documentCount; i++)
            documents.add(new Document(lines.get(i + 2), fragmentSize));

        DocumentList documentList = new DocumentList(documents, processors);
        documentList.computeRanks();

        List<Rank> ranks = documentList.getRanks();
        ranks.sort(new Comparator<Rank>() {
            public int compare(Rank o1, Rank o2) {
                return Float.compare(o2.getValue(), o1.getValue());
            }
        });
        String output = new Reduce<Rank, String>(ranks, "") {
            public String reduce(Rank rank, String acc) {
                return acc + rank.format() + '\n';
            }
        }.getResult();

        try {
            PrintWriter out = new PrintWriter(outFile);
            out.print(output);
            out.close();
        } catch (Exception e) { System.out.println(e.toString()); }
    }
}



