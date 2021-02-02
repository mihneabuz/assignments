import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.ConcurrentHashMap;

public class AuctionHouse {
    private static AuctionHouse INSTANCE;
    public static final int PRODUCT_CAPACITY = 10;
    public static final int MAX_AUCTIONS = 5;
    public static final int MAX_BIDS = 20;
    public static final int MAX_AUCTION_DURATION = 60;
    private final ConcurrentHashMap<Integer, Product> products = new ConcurrentHashMap<>(PRODUCT_CAPACITY);
    private final ArrayList<Client> clients = new ArrayList<>();
    private final ArrayList<Broker> brokers = new ArrayList<>();
    private final ArrayList<Administrator> admins = new ArrayList<>(PRODUCT_CAPACITY);
    private final ArrayList<Auction> auctions = new ArrayList<>();
    private int lastClientID = 1;
    private int lastAuctionID = 1;

    private AuctionHouse() {
    }

    public static AuctionHouse getINSTANCE() {
        if (INSTANCE == null)
            INSTANCE = new AuctionHouse();
        return INSTANCE;
    }

    public synchronized int requestNewClientID() {
        return lastClientID++;
    }


    public void addProduct(Product product) {
        products.put(product.getID(), product);
        assert products.size() <= AuctionHouse.PRODUCT_CAPACITY;
    }

    public Product removeProduct(int ID) {
        return products.remove(ID);
    }

    public void addAdministrator(Administrator admin) {
        if (admins.size() < AuctionHouse.PRODUCT_CAPACITY)
            admins.add(admin);
    }

    public synchronized void addClient(Client client) {
        clients.add(client);
    }

    public void addBroker(Broker broker) {
        brokers.add(broker);
    }

    public Product getProduct(int i) {
        return products.get(i);
    }

    public ArrayList<Product> getProducts() {
        return new ArrayList<>(products.values());
    }

    public Administrator getAdministrator() {
        return admins.get(new Random().nextInt(admins.size()));
    }

    public ArrayList<Client> getClients() {
        return clients;
    }

    public Broker requestBroker() throws NoBrokersAvailableException {
        for (Broker broker : brokers)
            if (!broker.isBusy()) {
                return broker;
            }
        throw new NoBrokersAvailableException("No brokers are currently available");
    }

    public void addAuction(Auction auction) {
        auctions.add(auction);
    }

    public ArrayList<Auction> getAuctions() {
        return auctions;
    }

    public ArrayList<Auction> getActiveAuctions() {
        ArrayList<Auction> activeAuctions = new ArrayList<>();
        for (int i = 0; i < auctions.size(); i++) {
            Auction auction = auctions.get(i);
            if (!auction.isFinished())
                activeAuctions.add(auction);
        }
        return activeAuctions;
    }

    public int nextAuctionID() {
        return lastAuctionID++;
    }

    public void printProducts() {
        for (Product product : products.values())
            System.out.println(product.toString());
    }

    public void debug() {
        StringBuilder string = new StringBuilder("Products: ");
        for (Product product : products.values())
            string.append(product.getID()).append(" ");
        string.append("\nOpen auctions: ");
        for (Auction auction : getActiveAuctions())
            string.append("\nAuction ").append(auction.toString());
        string.append("\n\nAdmins: ");
        for (Administrator admin : admins)
            string.append("\n").append(admin.toString());
        string.append("\n\nBrokers: ");
        for (Broker broker : brokers)
            string.append("\n").append(broker.toString());
        string.append("\n\nClients: ");
        for (Client client : clients)
            string.append("\n").append(client.toString());
        System.err.println(string);
    }
}
