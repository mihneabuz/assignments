import java.util.ArrayList;

class Fibonacci {
    private static int computed;
    private static ArrayList<Long> cache;

    static {
        cache = new ArrayList<>();
        cache.add(0l);
        cache.add(1l);
        computed = 2;
    }

    synchronized public static Long get(int index) {
        if (computed <= index) {
            for (int i = computed; i <= index; i++)
                cache.add(cache.get(i - 1) + cache.get(i - 2));

            computed = index + 1;
        }


        return cache.get(index);
    }
}
