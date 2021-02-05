package Main;

public class Register implements Runnable {
    private final AuctionHouse auctionHouse;
    private final Client client;

    public Register(AuctionHouse ah, Client client) {
        this.auctionHouse = ah;
        this.client = client;
    }

    @Override
    public void run() {
        client.setID(auctionHouse.requestNewClientID());
        auctionHouse.addClient(client);
        System.err.println("(+) New client registered: " + client.toString());
    }
}
