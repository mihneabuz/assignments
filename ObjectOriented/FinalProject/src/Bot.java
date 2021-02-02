import java.util.Random;

public class Bot extends Client {
    private int bidChance;
    private int maxRaiseAmount;

    public Bot(int ID, String name, String address) {
        super(ID, name, address);
    }

    @Override
    public void bid(double price) {
        if (super.broker == null || !broker.isAuctionOpen() || broker.winning())
            return;
        if (new Random().nextInt(100) > bidChance)
            return;
        double currentPrice = broker.getCurrentPrice();
        double newPrice = currentPrice + 20.0 + (new Random().nextInt(maxRaiseAmount)) / 1.3;
        broker.notifyBid(newPrice);
        bidChance -= 1;
    }

    @Override
    public void enterAuction(Auction auction) {
        bidChance = new Random().nextInt(8) + 2;
        maxRaiseAmount = new Random().nextInt(79) + 1;
        if (broker == null)
            return;
        broker.setAuction(auction);
        auction.notifyNewParticipant();
        System.out.println(super.getName() + " has entered auction " + auction.getID());
    }

    @Override
    public double getCommission() {
        return 0.05;
    }
}
