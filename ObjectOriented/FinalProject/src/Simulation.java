import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;

public class Simulation {
    private static int lastID = 0;

    private static synchronized int inc() {
        return lastID++;
    }

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
                    .withID(inc())
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

    public static Product getRandomProductFromAH(AuctionHouse auctionHouse) {
        ArrayList<Product> products = auctionHouse.getProductsOnSale();
        if (products.size() > 0)
            return (Product) products.get(new Random().nextInt(products.size()));
        return null;
    }

    public static void main(String[] args) throws InterruptedException {
        AuctionHouse ah = AuctionHouse.getINSTANCE();
        Administrator admin1 = new Administrator(0, "Franz", ah);
        Administrator admin2 = new Administrator(1, "Huan", ah);
        ArrayBlockingQueue<Product> products = new ArrayBlockingQueue<Product>(100);

        for (int i = 0; i < 30; i ++) {
            Product p = generateRandomProduct();
            products.add(p);
            new Thread(new Listing(admin1, p)).start();
            if (i > 5)
                new Thread(new Delisting(admin2, products.take())).start();
        }
        for (int i = 0; i < 6; i ++)
            new Thread(new Delisting(admin2, products.take())).start();

    }
}
