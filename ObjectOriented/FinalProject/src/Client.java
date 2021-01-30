public class Client {
    private int ID;
    private String name;
    private String address;
    private int participatedAuctions;
    private int wonAuctions;

    public Client() {
    }

    public Client(int ID, String name, String address, int participatedAuctions, int wonAuctions) {
        this.ID = ID;
        this.name = name;
        this.address = address;
        this.participatedAuctions = participatedAuctions;
        this.wonAuctions = wonAuctions;
    }

    public void requestBroker(AuctionHouse ah) {

    }
}
