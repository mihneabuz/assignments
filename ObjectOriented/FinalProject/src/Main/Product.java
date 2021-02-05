package Main;

public class Product {
    private int ID;
    private String name;
    private double startPrice;
    private boolean inAuction;
    private boolean inQueueForAuction;
    private double soldPrice;

    public Product() {
        this.inAuction = false;
        this.inQueueForAuction = false;
        this.soldPrice = 0;
    }

    public Product(int ID, String name, double startPrice) {
        this.ID = ID;
        this.name = name;
        this.startPrice = startPrice;
        this.inAuction = false;
        this.soldPrice = 0;
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

    public void setInQueueForAuction(boolean inQueueForAuction) {
        this.inQueueForAuction = inQueueForAuction;
    }

    public boolean isInQueueForAuction() {
        return inQueueForAuction;
    }

    public int getID() {
        return this.ID;
    }

    public double getStartPrice() {
        return startPrice;
    }

    public void setSoldPrice(double soldPrice) {
        this.soldPrice = soldPrice;
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
