public class Broker extends Employee {
    private Client client;
    private Auction auction;
    private boolean busy;

    public Broker(int ID, String name) {
        super(ID, name);
        this.busy = false;
    }

    private void notifyBid() {
    }

    public void setBusy() {
        this.busy = true;
    }

    public boolean isBusy() {
        return busy;
    }

    private double getCurrentPrice() {
        return auction.getCurrentPrice();
    }

    public String toString() {
        return "Broker " + super.toString() + (busy ? "(working)" : "(waiting)");
    }
}
