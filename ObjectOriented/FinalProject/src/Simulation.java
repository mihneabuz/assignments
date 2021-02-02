import java.util.ArrayList;
import java.util.Random;

public class Simulation {
    private static final int MAX_PRODUCTS = 20;
    private static final int MAX_CLIENTS = 30;
    private static int lastID = 0;
    private static int lastEmployeeID = 0;
    private static String[] namesPool = new String[]{"Franz", "Hanz", "Joseph", "Gigel", "Martin",
                                                     "Huan", "Arnold", "Dragnea", "Becali", "Giovanni",
                                                     "Nicu", "Lili", "Paulette", "Mathias", "Petrica",
                                                     "Ionut", "Jhon", "Sergiu", "Jesus", "Florin"};

    public static Product generateRandomProduct() {
        Random x = new Random();

        String[] types = new String[]{"PAINTING", "FURNITURE", "JEWELLERY"};
        int c1 = x.nextInt(3);

        String[] names = new String[]{"John", "A Stary Night", "Van Gogh", "Mone Lisa", "Hatz Jhonule", "Antilopa",
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
        return new Administrator(lastEmployeeID++, namesPool[new Random().nextInt(namesPool.length)], ah);
    }

    public static Broker generateRandomBroker() {
        return new Broker(lastEmployeeID++, namesPool[new Random().nextInt(namesPool.length)]);
    }

    public static Client generateRandomBot(AuctionHouse ah) {
        int c1 = new Random().nextInt(namesPool.length);

        String[] addresses = new String[]{"USA", "Africa", "Caransebes", "Vaslui", "Ardeal", "Rrrrusia", "Germoney",
                                          "Greece", "Pastaland", "Croissant", "The Motherland", "Cazanesti"};
        int c2 = new Random().nextInt(addresses.length);

        return new Bot(0, namesPool[c1] + " Bot", addresses[c2]);
    }

    public static Product getRandomProductForAuction(AuctionHouse auctionHouse) {
        ArrayList<Product> products = auctionHouse.getProducts();
        int i = new Random().nextInt(products.size());
        if (!products.get(i).isInAuction())
            return products.get(i);
        while (i < 2 * products.size()) {
            if (!products.get(i % products.size()).isInAuction())
                return products.get(i % products.size());
            i++;
        }
        return null;
    }

    public static Auction getRandomOpenAuction(AuctionHouse auctionHouse) {
        ArrayList<Auction> auctions = auctionHouse.getAuctions();
        return auctions.get(0);
    }

    public static void main(String[] args) {
        AuctionHouse ah = AuctionHouse.getINSTANCE();

        int step = 1;
        int nrProducts = 0;
        int nrClients = 0;
        int nrBrokers = 0;
        Random seed = new Random();
        for (int i = 0; i < 3; i++)
            ah.addAdministrator(generateRandomAdministrator(ah));

        while (step < 100000) {

          //  if (step == 20)
          //      new Thread(new Login(ah)).start();
            if (nrProducts < MAX_PRODUCTS && seed.nextInt(100) > 90) {
                nrProducts++;
                Product product = generateRandomProduct();
                Administrator admin = ah.getAdministrator();
                new Thread(new Listing(admin, product)).start();
            }
            if (nrClients < MAX_CLIENTS && seed.nextInt(100) > 90) {
                nrClients++;
                Client client = generateRandomBot(ah);
                new Thread(new Register(ah, client)).start();
            }
            if (nrBrokers < MAX_CLIENTS && seed.nextInt(100) > 80) {
                nrBrokers++;
                Broker broker = generateRandomBroker();
                ah.addBroker(broker);
            }
            if (ah.getProducts().size() > 0 && seed.nextInt(100) > 95) {
                Product p = getRandomProductForAuction(ah);
                if (p != null) {
                    Administrator admin = ah.getAdministrator();
                    admin.openAuction(p);
                }
            }
            ArrayList<Auction> activeAuctions = ah.getActiveAuctions();
            if (activeAuctions.size() > 0 && seed.nextInt(100) > 80) {
                Auction auction = activeAuctions.get(seed.nextInt(activeAuctions.size()));
                for (int i = 0; i < ah.getClients().size(); i++) {
                    Client client = ah.getClients().get(i);
                    if (client instanceof Bot && client.getBroker() == null && seed.nextInt(100) > 85) {
                        client.requestBroker(ah);
                        client.enterAuction(auction);
                    }
                }
            }
            for (int i = 0; i < ah.getClients().size(); i++) {
                Client client = ah.getClients().get(i);
                if (client instanceof Bot) {
                    if (client.hasBroker() && client.broker.isAuctionFinished())
                        client.dismissBroker();
                    else
                        client.bid(0);
                }
            }

            if (step % 500 == 0)
                ah.debug();
            step++;
            //Thread.sleep(30);
        }

    }
}
