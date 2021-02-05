package Main;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Bid implements Runnable {
    private final Auction auction;
    private final Broker broker;
    private final double newBid;
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
            System.err.println("!! Auction " + auction.getID() + ": new bid of " +
                    String.format("%.2f", newBid) + " from " + broker.getClient().getName());
        }
        lock.unlock();
    }
}
