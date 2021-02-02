public class Client {
    private int ID;
    private String name;
    private String address;
    protected Broker broker;
    private int participatedAuctions;
    private int wonAuctions;

    public Client() {
    }

    public Client(int ID, String name, String address) {
        this.ID = ID;
        this.name = name;
        this.address = address;
        this.participatedAuctions = 0;
        this.wonAuctions = 0;
        this.broker = null;
    }

    public void setID(int ID) {
        this.ID = ID;
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
            e.printStackTrace();
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
        auction.notifyNewParticipant();
        participatedAuctions += 1;
        System.out.println(name + " has entered auction " + auction.getID());
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
        return ID + " " + name + " from " + address;
    }
}
