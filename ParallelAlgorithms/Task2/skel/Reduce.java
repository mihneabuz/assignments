import java.util.List;

abstract class Reduce<T, L> {
    private List<T> input; 
    private L current;

    public Reduce(List<T> input, L initial) {
        this.input = input;
        this.current = initial;
    }

    public L getResult() {
        for (T val : input)
            current = reduce(val, current);
        return current;
    }

    public abstract L reduce(T input, L accumulator);
}
