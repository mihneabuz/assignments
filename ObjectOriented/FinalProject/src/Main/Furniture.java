package Main;

public class Furniture extends Product {
    private String type;
    private String material;

    public Furniture() {
        super();
    }

    public Furniture(int ID, String name, double startPrice, String type, String material) {
        super(ID, name, startPrice);
        this.type = type;
        this.material = material;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setMaterial(String material) {
        this.material = material;
    }

    public String toString() {
        return String.format("%s made out of %s | ", type, material) + super.toString();
    }
}
