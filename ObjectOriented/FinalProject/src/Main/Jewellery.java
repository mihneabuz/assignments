package Main;

public class Jewellery extends Product {
    private String material;
    private boolean withGem;

    public Jewellery() {
        super();
    }

    public Jewellery(int ID, String name, double startPrice, String material, boolean withGem) {
        super(ID, name, startPrice);
        this.material = material;
        this.withGem = withGem;
    }

    public void setMaterial(String material) {
        this.material = material;
    }

    public void setWithGem(boolean withGem) {
        this.withGem = withGem;
    }

    public String toString() {
        return String.format("Main.Jewellery made of %s %s Gems\n", material, withGem ? "with" : "without")
                + super.toString();
    }
}
