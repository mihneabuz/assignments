package Main;

public class Client {
    private int ID;
    private String name;
    private String address;
    protected Broker broker;
    private double credit;
    private int participatedAuctions;
    private int wonAuctions;

    public Client() {
    }

    public Client(String name, String address) {
        this.ID = 0;
        this.name = name;
        this.address = address;
        this.credit = 0;
        this.participatedAuctions = 0;
        this.wonAuctions = 0;
        this.broker = null;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public void setCredit(double credit) {
        this.credit = credit;
    }

    public double getCredit() {
        return credit;
    }

    public Broker getBroker() {
        return broker;
    }

    public boolean hasBroker() {
        return broker != null;
    }

    public String getName() {
        return name;
    }

    public void requestBroker(AuctionHouse ah) {
        try {
            broker = ah.requestBroker();
            broker.setClient(this);
        } catch (NoBrokersAvailableException e) {
            System.out.println(e.getMessage());
        }
    }

    public void dismissBroker() {
        if (broker == null)
            return;
        broker.dismiss();
        broker = null;
    }

    public double requestCurrentPrice() {
        if (broker == null)
            return -1;
        return broker.getCurrentPrice();
    }

    public void enterAuction(Auction auction) {
        if (broker == null)
            return;
        broker.setAuction(auction);
        auction.notifyNewParticipant(name);
        participatedAuctions += 1;
    }

    public void bid(double price) {
        if (broker == null)
            return;
        broker.notifyBid(price);
    }

    public int getWonAuctions() {
        return wonAuctions;
    }

    public void setWonAuctions(int wonAuctions) {
        this.wonAuctions = wonAuctions;
    }

    public double getCommission() {
        return 0.1;
    }

    public String toString() {
        return name + " from " + address;
    }
}
