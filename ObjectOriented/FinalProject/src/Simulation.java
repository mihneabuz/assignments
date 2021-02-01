import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;

public class Simulation {
    private static final int MAX_PRODUCTS = 10;
    private static final int MAX_CLIENTS = 20;
    private static int lastID = 0;
    private static int lastAdminID = 0;
    private static int lastClientID = 0;


    public static Product generateRandomProduct() {
        Random x = new Random();

        String[] types = new String[]{"PAINTING", "FURNITURE", "JEWELLERY"};
        int c1 = x.nextInt(3);

        String[] names = new String[]{"John", "A Stary Night", "Van Gogh", "Mone Lisa", "Hatz Jhonule", "danutzu",
                                      "Ikea Table", "Fotoliul", "Inelush", "EXPLOSIONS?", "$GME", "HOLD"};
        int c2 = x.nextInt(names.length);

        String[] artists = new String[]{"Aurel", "Lili", "danutzul", "Jhonul", "LorinFortuna", "VadimTudor", "DFV"};
        int c3 = x.nextInt(artists.length);

        String[] colors = new String[]{"TEMPERA", "OIL", "ACRYLIC"};
        int c4 = x.nextInt(3);

        String[] furnitures = new String[]{"Masa", "Fotoliu", "Canapea", "Dulap", "Sifonier"};
        int c5 = x.nextInt(furnitures.length);

        String[] materials = new String[]{"Haur", "Argint", "Lemn", "PVC", "Peatra", "Platinium", "Fer"};
        int c6 = x.nextInt(materials.length);

        Product p;
        try {
            p = new ProductBuilder(types[c1])
                    .withID(lastID++)
                    .withName(names[c2])
                    .withStartPrice(x.nextInt(200))
                    .withArtist(artists[c3])
                    .withColors(colors[c4])
                    .withFurnitureType(furnitures[c5])
                    .withMaterial(materials[c6])
                    .setWithGem(x.nextInt(2) > 0)
                    .build();
        } catch (InvalidProductException e) {
            e.printStackTrace();
            return null;
        }
        return p;
    }

    public static Administrator generateRandomAdministrator(AuctionHouse ah) {
        String[] names = new String[]{"Franz", "Hanz", "Joseph", "Gigel", "Martin", "Huan", "Arnold", "Dragnea"};
        return new Administrator(lastAdminID++, names[new Random().nextInt(names.length)], ah);
    }

    public static Client generateRandomBot(AuctionHouse ah) {
        String[] names = new String[]{"Franz", "Hanz", "Joseph", "Gigel", "Martin", "Huan", "Arnold", "Dragnea"};
        int c1 = new Random().nextInt(names.length);

        String[] addresses = new String[]{"USA", "Africa", "Caransebes", "Vaslui", "Ardeal", "Rrrrusia", "Germoney",
                                          "Greece", "Pastaland", "Croissant", "The Motherland"};
        int c2 = new Random().nextInt(addresses.length);

        return new Bot(lastClientID++, names[c1] + " Bot", addresses[c2]);
    }

    public static Product getRandomProductFromAH(AuctionHouse auctionHouse) {
        ArrayList<Product> products = auctionHouse.getProductsOnSale();
        if (products.size() > 0)
            return (Product) products.get(new Random().nextInt(products.size()));
        return null;
    }

    public static void main(String[] args) throws InterruptedException {
        AuctionHouse ah = AuctionHouse.getINSTANCE();

        int step = 1;
        int nrProducts = 0;
        int nrClients = 0;
        Random seed = new Random();
        for (int i = 0; i < AuctionHouse.PRODUCT_CAPACITY; i++)
            ah.addAdministrator(generateRandomAdministrator(ah));

        while(true) {
            if (nrProducts < MAX_PRODUCTS && seed.nextInt(100) > 90) {
                nrProducts++;
                Product product = generateRandomProduct();
                Administrator admin = ah.getAdministrator();
                new Thread(new Listing(admin, product)).start();
            }
            if (nrClients < MAX_CLIENTS && seed.nextInt(100) > 95) {
                nrClients++;
                Client client = generateRandomBot(ah);
                new Thread(new Register(ah, client)).start();
            }
            if (ah.getProductsOnSale().size() > 0 && seed.nextInt(100) > 90) {
                Product p = getRandomProductFromAH(ah);
                Administrator admin = ah.getAdministrator();
                admin.openAuction(p);
            }



            if (step % 50 == 0)
                ah.debug();
            step++;
            Thread.sleep(300);
        }

    }
}
