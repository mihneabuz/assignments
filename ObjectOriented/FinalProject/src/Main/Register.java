package Main;

public class Register implements Runnable {
    private AuctionHouse auctionHouse;
    private Client client;

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
