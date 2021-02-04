package CommandLineInterface;

import Main.Auction;
import Main.AuctionHouse;
import Main.Client;
import Main.InvalidAuctionIDException;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.security.InvalidParameterException;

public class EnterAuction implements Command{
    @Override
    public void execute(Client client, String[] arguments) throws InvalidParameterException {
        if (!client.hasBroker()) {
            System.out.println("You have no broker assigned currently");
            return;
        }
        if (arguments.length < 2)
            throw new InvalidParameterException("enter requires 1 argument [auction ID]");
        int auctionID;
        Auction auction;
        try {
            auctionID = Integer.parseInt(arguments[1]);
            auction = AuctionHouse.getINSTANCE().getAuction(auctionID);
            client.enterAuction(auction);
        } catch (NumberFormatException e) {
            throw new InvalidParameterException("argument is not a number");
        } catch (InvalidAuctionIDException e) {
            System.out.println(e.getMessage());
            return;
        }

        if (!auction.isOpen())
            System.out.println("Waiting for more participants...");
        while (!auction.isOpen())
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        System.out.println("Entered auction for " + auction.getProduct().toString() +
                "\nPress Enter and type in amount to bid. Enter q to exit auction");
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        double newPrice;
        double lastPrice = 0;
        double yourBid = 0;
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        try {
            while (!auction.isFinished()) {
                while (System.in.available() == 0) {
                    newPrice = client.getBroker().getCurrentPrice();
                    if (newPrice != lastPrice) {
                        System.out.println("New price: " + newPrice + (newPrice == yourBid ? " (your bid)" : ""));
                        lastPrice = newPrice;
                    }
                    try {
                        Thread.sleep(200);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                reader.readLine();
                System.out.print("Your bid: ");
                String aux = reader.readLine();
                if (aux.equals("q")) {
                    System.out.println("Quit auction " + auction.getID());
                    return;
                }

                yourBid = Double.parseDouble(aux);
                if (yourBid > client.getCredit())
                    System.out.println("Rejected. Not enough credit to bid");
                else
                    client.bid(yourBid);

            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("Auction finished\n");
        if (auction.getHighestBidder().is(client.getBroker())) {
            System.out.println("You won " + auction.getProduct().toString() + " for " + auction.getCurrentPrice());
            System.out.println("Available credit: " + client.getCredit());
        }
        else {
            System.out.println("Sold " + auction.getProduct().toString() + " for " + auction.getCurrentPrice());
        }
    }
}
