package Main;

import java.util.Random;

public class Bot extends Client {
    private int bidChance;
    private int maxRaiseAmount;

    public Bot(String name, String address) {
        super(name, address);
    }

    @Override
    public void bid(double price) {
        if (getBroker() == null || !getBroker().isAuctionOpen() || getBroker().winning())
            return;
        if (new Random().nextInt(100) > bidChance)
            return;
        double currentPrice = getBroker().getCurrentPrice();
        double newPrice = currentPrice + 20.0 + ((float)new Random().nextInt(maxRaiseAmount * 100) / 100);
        if (newPrice <= getCredit())
            getBroker().notifyBid(newPrice);
        bidChance -= 1;
    }

    @Override
    public void enterAuction(Auction auction) {
        assert auction != null;
        bidChance = new Random().nextInt(10) + 2;
        maxRaiseAmount = new Random().nextInt(49) + 1;
        setCredit(new Random().nextInt(maxRaiseAmount * 20) + 300);
        if (getBroker() == null)
            return;
        getBroker().setAuction(auction);
        auction.notifyNewParticipant(getName());
    }

    @Override
    public double getCommission() {
        return 0.05;
    }
}
