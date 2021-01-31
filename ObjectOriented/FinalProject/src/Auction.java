public class Auction {
    private int ID;
    private int noPraticipants;
    private int productID;
    private int currentPrice;
    private int maxBids;

    public Auction() {
    }

    public synchronized void step() {
        maxBids--;
    }

    public int getCurrentPrice() {
        return currentPrice;
    }
}
