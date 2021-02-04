package Main;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Auction implements Runnable {
    private final AuctionHouse auctionHouse;
    private final int ID;
    private final Product product;
    private int noParticipants;
    private double currentPrice;
    private Broker highestBidder;
    private int step;
    private int maxBids;
    private final AuctionStatus status;

    static Lock lock = new ReentrantLock();
    static Condition auctionHouseNotFull = lock.newCondition();
    Condition enoughParticipants = lock.newCondition();

    public Auction(int ID, Product product, AuctionHouse auctionHouse) {
        this.auctionHouse = auctionHouse;
        this.ID = ID;
        this.product = product;
        this.noParticipants = 0;
        this.currentPrice = product.getStartPrice();
        this.highestBidder = null;
        this.step = 0;
        this.maxBids = AuctionHouse.MAX_BIDS;
        this.status = new AuctionStatus();
    }

    public synchronized void step() {
        maxBids--;
    }

    public Product getProduct() {
        return product;
    }

    public double getCurrentPrice() {
        return currentPrice;
    }

    public synchronized void notifyNewParticipant(String name) {
        lock.lock();
        System.err.println("<+> " + name + " has entered auction " + this.getID());
        noParticipants++;
        if (noParticipants >= AuctionHouse.MIN_AUCTION_PARTICIPANTS)
            enoughParticipants.signal();
        lock.unlock();
    }

    public int getID() {
        return ID;
    }

    public Broker getHighestBidder() {
        return highestBidder;
    }

    public int getNoParticipants() {
        return noParticipants;
    }

    public int getTimeRemaining() {
        return AuctionHouse.MAX_AUCTION_DURATION - step;
    }

    public void setCurrentPrice(double currentPrice) {
        this.currentPrice = currentPrice;
    }

    public void setHighestBidder(Broker broker) {
        this.highestBidder = broker;
    }

    public boolean isOpen() {
        return status.isOpen();
    }

    public boolean isFinished() {
        return status.isFinished();
    }

    public String toString() {
        return ID + " for product " + product.getID() + " participants: " + noParticipants +
                " time remaining: " + getTimeRemaining();
    }

    @Override
    public void run() {
        lock.lock();
        if (auctionHouse.getActiveAuctions().size() >= AuctionHouse.MAX_AUCTIONS) {
            try {
                auctionHouseNotFull.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        auctionHouse.addAuction(this);
        product.setInAuction();

        System.err.println("\n<=> New auction opened for item:\n" + product.toString() + "\n");
        if (noParticipants < AuctionHouse.MIN_AUCTION_PARTICIPANTS) {
            try {
                enoughParticipants.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        status.setOpen(true);
        lock.unlock();

        System.err.println("\n<=> Auction " + ID + " has started\n");

        while(maxBids > 0 && step < AuctionHouse.MAX_AUCTION_DURATION) {
            step++;
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        lock.lock();
        status.setOpen(false);
        status.setFinished(true);
        auctionHouseNotFull.signal();
        if (highestBidder == null) {
            System.err.println("\n<=> Auction " + ID + " finished: Product not sold.\n");
        }
        else {
            Client winner = highestBidder.getClient();
            status.setWinner(winner);
            status.setBroker(highestBidder);

            winner.setCredit(winner.getCredit() - currentPrice);
            winner.setWonAuctions(winner.getWonAuctions() + 1);
            highestBidder.setCommission(highestBidder.getCommission() + currentPrice * winner.getCommission());

            System.err.println("\n<=> Auction " + ID + " finished: Sold to " + status.getWinner().getName() +
                                " for " + String.format("%.2f", currentPrice) + "\n");
        }
        Administrator admin = auctionHouse.getAdministrator();
        new Thread(new Delisting(admin, product)).start();
        lock.unlock();
    }

}
