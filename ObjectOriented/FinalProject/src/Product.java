public class Product {
    private int ID;
    private String name;
    private double startPrice;
    private boolean sold;
    private double soldPrice;

    public Product() {
        this.sold = false;
        this.soldPrice = Integer.MIN_VALUE;
    }

    public Product(int ID, String name, double startPrice) {
        this.ID = ID;
        this.name = name;
        this.startPrice = startPrice;
        this.sold = false;
        this.soldPrice = Integer.MIN_VALUE;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setStartPrice(double startPrice) {
        this.startPrice = startPrice;
    }

    public void setSold(double price) {
        this.sold = true;
        this.soldPrice = price;
    }

    public boolean wasSold() {
        return this.sold;
    }

    public String toString() {
        StringBuilder string = new StringBuilder();
        string.append(String.format("Product: %d\n%s\n", ID, name));
        if (this.sold)
            string.append(String.format("Sold for %.2f", soldPrice));
        else
            string.append(String.format("Starting price %.2f", startPrice));
        return string.toString();
    }
}
