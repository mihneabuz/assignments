public class Auction implements Runnable {
    private int ID;
    private Product product;
    private int noPraticipants;
    private double currentPrice;
    private int maxBids;
    private AuctionStatus status;

    public Auction() {
    }

    public Auction(int ID, Product product, int maxBids) {
        this.ID = ID;
        this.product = product;
        this.noPraticipants = 0;
        this.currentPrice = product.getStartPrice();
        this.maxBids = maxBids;
        this.status = new AuctionStatus();
    }

    public synchronized void step() {
        maxBids--;
    }

    public double getCurrentPrice() {
        return currentPrice;
    }

    @Override
    public void run() {
    }
}
