package CommandLineInterface;

import Main.AuctionHouse;
import Main.Client;

public class ListProducts implements Command{

    @Override
    public void execute(Client client, String[] arguments) {
        AuctionHouse auctionHouse = AuctionHouse.getINSTANCE();
        auctionHouse.printProducts();
    }
}
