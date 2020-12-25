import java.io.IOException;
import java.io.Serializable;

public class Manufacturer {
    private String name;
    private int countProducts;

    public Manufacturer() {
    }

    public Manufacturer(String name) {
        this.name = name;
        this.countProducts = 0;
    }

    public String getName() {
        return name;
    }

    public void incCount() {
        countProducts ++;
    }

    public void decCount() { countProducts --; }

    public String toString() {
        return "Manufacturer " + name + " with " + countProducts + " products";
    }

    public void writeBin(BinaryOutputStream writer) throws IOException {
        writer.write(name);
        writer.write(countProducts);
    }
}
