import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

class DocumentList {
    private ArrayList<Document> docs;
    private int threads;

    public DocumentList(ArrayList<Document> docs, int threads) {
        this.docs = docs;
        this.threads = threads;
    }

    class MapArgs {
        Document doc;
        int fragIndex;

        public MapArgs(Document doc, int fragIndex) {
            this.doc = doc;
            this.fragIndex = fragIndex;
        }
    }

    public List<Fragment> computeFragments() {
        ArrayList<MapArgs> input = new ArrayList<>();
        for (Document doc : docs)
            for (int i = 0; i < doc.getFragmentCount(); i++)
                input.add(new MapArgs(doc, i));

        Map<MapArgs, Fragment> fragMap = new Map<>(input, threads) {
            public Fragment map(MapArgs args) {
                return new Fragment(args.doc.getName(), args.doc.getFragment(args.fragIndex));
            }
        };

        return fragMap.getResult();
    }

    public void calculateRanks(List<Fragment> fragments) {
        ExecutorService executor = Executors.newFixedThreadPool(threads); 

        int start = 0;
        for (Document doc : docs) {
            executor.submit(new RankTask(doc, fragments.subList(start, start + doc.getFragmentCount())));
            start += doc.getFragmentCount();
        }

        executor.shutdown();
        try {
            executor.awaitTermination(Integer.MAX_VALUE, TimeUnit.NANOSECONDS);
        } catch (Exception e) { System.out.println(e.toString()); }
    }

    class RankTask implements Runnable {
        Document doc;
        List<Fragment> fragments;    

        public RankTask(Document doc, List<Fragment> fragments) {
            this.doc = doc;
            this.fragments = fragments;
        }

        public void run() {
            Reduce<Fragment, Rank> reduce = new Reduce<>(fragments, new Rank()) {
                public Rank reduce(Fragment frag, Rank acc) {
                    acc.combine(frag);
                    return acc;
                }
            };

            Rank result = reduce.getResult();
            result.compute();
            doc.setRank(result);
        }
    }

    public void computeRanks() {
        List<Fragment> fragments = computeFragments();
        calculateRanks(fragments);
    }

    public List<Rank> getRanks() {
        return new Map<Document, Rank>(docs, 1) {
            public Rank map(Document doc) {
                return doc.getRank();
            }
        }.getResult();
    }
}
