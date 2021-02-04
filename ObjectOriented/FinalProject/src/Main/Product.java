package Main;

public class Product {
    private int ID;
    private String name;
    private boolean onAuction;
    private double startPrice;
    private boolean inAuction;
    private double soldPrice;

    public Product() {
        this.onAuction = false;
        this.inAuction = false;
        this.soldPrice = Integer.MIN_VALUE;
    }

    public Product(int ID, String name, double startPrice) {
        this.ID = ID;
        this.name = name;
        this.startPrice = startPrice;
        this.inAuction = false;
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

    public void setInAuction() {
        this.inAuction = true;
    }

    public boolean isInAuction() {
        return inAuction;
    }

    public int getID() {
        return this.ID;
    }

    public double getStartPrice() {
        return startPrice;
    }

    public String toString() {
        return name;
    }

    public String getStatus() {
        if (this.inAuction)
            return "\nCurrently in auction";
        else
            return String.format("\nStarting price %.2f", startPrice);
    }
}
