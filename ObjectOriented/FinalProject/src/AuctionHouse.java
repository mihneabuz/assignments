import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.ConcurrentHashMap;

public class AuctionHouse {
    private static AuctionHouse INSTANCE;
    public static final int PRODUCT_CAPACITY = 5;
    public static final int MAX_BIDS = 5;
    private final ConcurrentHashMap<Integer, Product> products = new ConcurrentHashMap<>(PRODUCT_CAPACITY);
    private final ArrayList<Client> clients = new ArrayList<>();
    private final ArrayList<Broker> brokers = new ArrayList<>();
    private final ArrayList<Administrator> admins = new ArrayList<>(PRODUCT_CAPACITY);
    private final ArrayList<Auction> auctions = new ArrayList<>();
    private int lastAuctionID = 0;

    private AuctionHouse() {
    }

    public static AuctionHouse getINSTANCE() {
        if (INSTANCE == null)
            INSTANCE = new AuctionHouse();
        return INSTANCE;
    }


    public ConcurrentHashMap<Integer, Product> getProducts() {
        return products;
    }

    public void addProduct(Product product) {
        products.put(product.getID(), product);
        assert products.size() <= AuctionHouse.PRODUCT_CAPACITY;
    }

    public void addAdministrator(Administrator admin) {
        if (admins.size() < AuctionHouse.PRODUCT_CAPACITY)
            admins.add(admin);
    }

    public synchronized void addClient(Client client) {
        clients.add(client);
    }

    public void addAuction(Auction auction) {
        auctions.add(auction);
    }

    public Product getProduct(int i) {
        return products.get(i);
    }

    public ArrayList<Product> getProductsOnSale() {
        ArrayList<Product> forSale = new ArrayList<>();
        for (Product product : products.values()) {
            if (!product.wasSold())
                forSale.add(product);
        }
        return forSale;
    }

    public Administrator getAdministrator() {
        for (Administrator admin : admins)
            if (!admin.isBusy())
                return admin;
        return null;
    }

    public ArrayList<Client> getClients() {
        return clients;
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
            string.append(product.getID() + " ");
        string.append("\nClients: ");
        for (Client client : clients)
            string.append("\n" + client.toString());
        System.err.println(string);
    }


    public static void main(String[] args) {
        AuctionHouse ah = AuctionHouse.getINSTANCE();
        Product p = null;
        try {
             p = new ProductBuilder("PAINTING")
                    .withID(2)
                    .withName("dabada")
                    .withStartPrice(3.50d)
                    .withArtist("Jhonul")
                    .withColors("TEMPERA")
                    .build();
        } catch (InvalidProductException e) {
            System.out.println(e);
        }
        System.out.println(p.toString() + "\n\n");
        ah.addProduct(p);
        ah.printProducts();

    }

}
