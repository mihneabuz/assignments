public class Broker extends Employee {
    private Client client;
    private Auction auction;

    private void notifyBid() {

    }

    private int getCurrentPrice() {
        return auction.getCurrentPrice();
    }
}
