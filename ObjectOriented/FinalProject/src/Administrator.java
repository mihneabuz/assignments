import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Administrator extends Employee {
    public AuctionHouse auctionHouse;
    private boolean busy = false;
    private Auction auction;
    private final Lock lock = new ReentrantLock();

    public Administrator(int ID, String name, AuctionHouse auctionHouse) {
        super(ID, name);
        this.auctionHouse = auctionHouse;
    }

    public boolean isBusy() {
        return busy;
    }

    public void addProduct(Product p) {
        lock.lock();
        try {
            Thread.sleep(300);
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
        try {
            Thread.sleep(1000);
            Product aux = auctionHouse.getProducts().remove(p.getID());
            assert aux != null;
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void openAuction(Product product) {
        System.out.println("New auction opened for item:\n" + product.toString());
        product.setInAuction();
        busy = true;
        Auction auction = new Auction(auctionHouse.nextAuctionID(), product, AuctionHouse.MAX_BIDS);
        auctionHouse.addAuction(auction);
        new Thread(auction).start();
        busy = false;
    }

    public String toString() {
        return "Administrator " + super.toString() + (busy ? "(working)" : "(waiting)");
    }
}
