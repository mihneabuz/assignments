package Main;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Administrator extends Employee {
    public AuctionHouse auctionHouse;
    private Auction auction;
    private final Lock lock = new ReentrantLock();
    Condition notFull = lock.newCondition();

    public Administrator(int ID, String name, AuctionHouse auctionHouse) {
        super(ID, name);
        this.auctionHouse = auctionHouse;
    }

    public void addProduct(Product p) {
        lock.lock();
        try {
            if (auctionHouse.getProducts().size() >= AuctionHouse.PRODUCT_CAPACITY)
                notFull.await();
            auctionHouse.addProduct(p);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void removeProduct(Product p) {
        lock.lock();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Product aux = auctionHouse.removeProduct(p.getID());
        notFull.signal();
        assert aux != null;
        lock.unlock();
    }

    public void openAuction(Product product) {
        auction = new Auction(auctionHouse.nextAuctionID(), product, auctionHouse);
        new Thread(auction).start();
    }

    public String toString() {
        return "Main.Administrator " + super.toString();
    }
}
