public class Broker extends Employee {
    private Client client;
    private Auction auction;

    private void notifyBid() {

    }

    private double getCurrentPrice() {
        return auction.getCurrentPrice();
    }
}
