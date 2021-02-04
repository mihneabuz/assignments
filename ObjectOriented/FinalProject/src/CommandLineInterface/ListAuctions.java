package CommandLineInterface;

import Main.Auction;
import Main.AuctionHouse;
import Main.Client;

import java.security.InvalidParameterException;
import java.util.ArrayList;

public class ListAuctions implements Command{
    @Override
    public void execute(Client client, String[] arguments) throws InvalidParameterException {
        ArrayList<Auction> auctions = AuctionHouse.getINSTANCE().getAuctions();
        StringBuilder string = new StringBuilder("Active auctions:\n");
        boolean listFinished = arguments.length > 1 && arguments[1].contains("a");
        for (Auction auction : auctions) {
            if (listFinished || !auction.isFinished()) {
                string.append("\nAuction ").append(auction.getID()).append("\n")
                        .append(auction.getProduct().toString()).append("\n");
                if (auction.isOpen())
                    string.append(String.format("Opened with %d participants", auction.getNoParticipants()))
                            .append(String.format(" Time remaining: %d", auction.getTimeRemaining()))
                            .append(" seconds\n");
                else if (!auction.isFinished())
                    string.append("Waiting for participants...\n");
            }
        }
        System.out.println(string);
    }
}
