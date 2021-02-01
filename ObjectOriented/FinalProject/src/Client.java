public class Client {
    private int ID;
    private String name;
    private String address;
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
    }

    public void requestBroker(AuctionHouse ah) {

    }

    public String toString() {
        return name + " from " + address;
    }
}
