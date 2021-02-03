package Main;

public class Broker extends Employee {
    private double commission;
    private Client client;
    private Auction auction;
    private boolean busy;

    public Broker(int ID, String name) {
        super(ID, name);
        this.busy = false;
    }

    public void notifyBid(double price) {
        new Thread(new Bid(auction, this, price)).start();
    }

    public void setBusy() {
        this.busy = true;
    }

    public boolean isBusy() {
        return busy;
    }

    public void setAuction(Auction auction) {
        this.auction = auction;
    }

    public void setClient(Client client) {
        this.client = client;
        this.busy = true;
    }

    public Client getClient() {
        return client;
    }

    public double getCommission() {
        return commission;
    }

    public void setCommission(double commission) {
        this.commission = commission;
    }

    public void dismiss() {
        this.client = null;
        this.busy = false;
    }

    public boolean isAuctionOpen() {
        return auction.isOpen();
    }

    public boolean isAuctionFinished() {
        return auction.isFinished();
    }

    public double getCurrentPrice() {
        return auction.getCurrentPrice();
    }

    public boolean winning() {
        Broker highestBidder = auction.getHighestBidder();
        return highestBidder != null && highestBidder.getID() == super.getID();
    }

    public String toString() {
        return "Main.Broker " + super.toString() + (busy ? " (working on " + auction.getID() + ")" : " (waiting)");
    }

}
