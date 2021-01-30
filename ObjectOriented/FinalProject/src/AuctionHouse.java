import java.util.ArrayList;

public class AuctionHouse {
    private static AuctionHouse INSTANCE;
    private static int PRODUCT_CAPACITY = 20;
    private ArrayList<Product> products;
    private ArrayList<Client> clients;
    private ArrayList<Auction> activeAuctions;

    private AuctionHouse() {
    }

    public static AuctionHouse getINSTANCE() {
        if (INSTANCE == null)
            INSTANCE = new AuctionHouse();
        return INSTANCE;
    }

    public ArrayList<Product> getProductsOnSale() {
        ArrayList<Product> forSale = new ArrayList<>();
        for (Product product : products)
            if (!product.wasSold())
                forSale.add(product);
        return forSale;
    }

    public ArrayList<Product> getProducts() {
        return products;
    }

    public void addProduct(Product product) {
        products.add(product);
    }

    public void printProducts() {
        for (Product product : products)
            System.out.println(product.toString());
    }


    public static void main(String[] args) {
        AuctionHouse ah = AuctionHouse.getINSTANCE();
        Product p = null;
        try {
             p = ProductFactory.buildProduct("PAINTING")
                    .withID(2)
                    .withName("dabada")
                    .withStartPrice(3.50d)
                    .withArtist("Jhonul")
                    .withColors("TEMPERA")
                    .build();
        } catch (InvalidProductException e) {
            System.out.println(e);
        }
        System.out.println(p.toString());
    }
}
