import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.*;

abstract class Map<T, L> {
    private List<T> input; 
    private List<L> result;
    private boolean multithreaded;
    private ExecutorService executor;

    public Map(List<T> input) {
        this.input = input;
        this.result = new ArrayList<>();
        for (int i = 0; i < input.size(); i++)
            this.result.add(null);
        this.multithreaded = false;
    }

    public Map(List<T> input, int threads) {
        this.input = input;
        this.result = new ArrayList<>();
        for (int i = 0; i < input.size(); i++)
            this.result.add(null);
        this.multithreaded = true;
        this.executor = Executors.newFixedThreadPool(threads);
    }

    public List<L> getResult() {
        if (multithreaded) {
            for (int i = 0; i < input.size(); i++)
                executor.submit(new MapTask(i));

            executor.shutdown();
            try {
                executor.awaitTermination(Integer.MAX_VALUE, TimeUnit.NANOSECONDS);
            } catch (Exception e) { System.out.println(e.toString()); }

        } else {
            for (int i = 0; i < input.size(); i++)
                result.set(i, map(input.get(i)));
        }

        return result;
    }

    class MapTask implements Runnable {
        private int index;

        public MapTask(int index) {
            this.index = index;
        }

        public void run() {
            result.set(index, map(input.get(index)));
        }
    }

    public abstract L map(T input);
}
