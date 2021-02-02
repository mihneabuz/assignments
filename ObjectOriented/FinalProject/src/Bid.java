import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Bid implements Runnable {
    private Auction auction;
    private Broker broker;
    private double newBid;
    Lock lock = new ReentrantLock();

    public Bid(Auction auction, Broker broker, double bidAmount) {
        this.auction = auction;
        this.broker = broker;
        this.newBid = bidAmount;
    }

    @Override
    public void run() {
        lock.lock();
        double currentBid = auction.getCurrentPrice();
        if (newBid > currentBid && !auction.isFinished()) {
            auction.setCurrentPrice(newBid);
            auction.setHighestBidder(broker);
            auction.step();
            System.out.println("New price for auction " + auction.getID() + ": " + newBid + " from " +
                               broker.getClient().getName());
        }
        lock.unlock();
    }
}
