import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;

public class AuctionHouse {
    private static AuctionHouse INSTANCE;
    public static int PRODUCT_CAPACITY = 10;
    private ConcurrentHashMap<Integer, Product> products = new ConcurrentHashMap<>();
    private HashMap<Integer, Client> clients = new HashMap<>();
    private ArrayList<Broker> brokers = new ArrayList<>();
    private ArrayList<Administrator> admins = new ArrayList<>();
    private ArrayList<Auction> activeAuctions = new ArrayList<>();

    private AuctionHouse() {
    }

    public static AuctionHouse getINSTANCE() {
        if (INSTANCE == null)
            INSTANCE = new AuctionHouse();
        return INSTANCE;
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

    public ConcurrentHashMap<Integer, Product> getProducts() {
        return products;
    }

    public void addProduct(Product product) {
        products.put(product.getID(), product);
        assert products.size() <= AuctionHouse.PRODUCT_CAPACITY;
    }

    public void printProducts() {
        for (Product product : products.values())
            System.out.println(product.toString());
    }

    public void debug() {
        StringBuilder string = new StringBuilder();
        for (Product product : products.values())
            string.append(product.getID() + " ");
        System.out.println(string);
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
