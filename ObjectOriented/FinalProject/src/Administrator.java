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
            while (auctionHouse.getProducts().size() >= AuctionHouse.PRODUCT_CAPACITY)
                Thread.sleep(100);
            auctionHouse.addProduct(p);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void removeProduct(Product p) {
        Product aux = auctionHouse.removeProduct(p.getID());
        assert aux != null;
    }

    public void openAuction(Product product) {
        auction = new Auction(auctionHouse.nextAuctionID(), product, auctionHouse);
        new Thread(auction).start();
    }

    public String toString() {
        return "Administrator " + super.toString();
    }
}
