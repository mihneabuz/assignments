import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Administrator extends Employee {
    public AuctionHouse auctionHouse;
    Lock lock = new ReentrantLock();

    public Administrator(int ID, String name, AuctionHouse auctionHouse) {
        super(ID, name);
        this.auctionHouse = auctionHouse;
    }

    public void addProduct(Product p) {
        lock.lock();
        auctionHouse.debug();
        try {
            Thread.sleep(100);
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
        lock.lock();
        auctionHouse.debug();
        try {
            Thread.sleep(300);
            auctionHouse.getProducts().remove(p.getID());
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void openAuction() {

    }
}
