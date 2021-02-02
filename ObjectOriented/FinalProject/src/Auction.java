public class Auction implements Runnable {
    private final AuctionHouse auctionHouse;
    private final int ID;
    private final Product product;
    private int noParticipants;
    private double currentPrice;
    private Broker highestBidder;
    private int maxBids;
    private final AuctionStatus status;

    public Auction(int ID, Product product, AuctionHouse auctionHouse) {
        this.auctionHouse = auctionHouse;
        this.ID = ID;
        this.product = product;
        this.noParticipants = 0;
        this.currentPrice = product.getStartPrice();
        this.highestBidder = null;
        this.maxBids = AuctionHouse.MAX_BIDS;
        this.status = new AuctionStatus();
    }

    public synchronized void step() {
        maxBids--;
    }

    public Product getProduct() {
        return product;
    }

    public double getCurrentPrice() {
        return currentPrice;
    }

    public synchronized void notifyNewParticipant() {
        noParticipants++;
    }

    public int getID() {
        return ID;
    }

    public Broker getHighestBidder() {
        return highestBidder;
    }

    public void setCurrentPrice(double currentPrice) {
        this.currentPrice = currentPrice;
    }

    public void setHighestBidder(Broker broker) {
        this.highestBidder = broker;
    }

    public boolean isOpen() {
        return status.isOpen();
    }

    public boolean isFinished() {
        return status.isFinished();
    }

    public String toString() {
        return ID + " for product " + product.getID() + " participants: " + noParticipants;
    }

    @Override
    public void run() {
        product.setInAuction();
        while (auctionHouse.getActiveAuctions().size() >= AuctionHouse.MAX_AUCTIONS)
            try {
                Thread.sleep(50);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        auctionHouse.addAuction(this);
        System.err.println("New auction opened for item:\n" + product.toString());
        while (noParticipants < 3)
            try {
                Thread.sleep(50);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        status.setOpen(true);
        System.out.println("Auction " + ID + " has started");

        int step = 0;
        while(maxBids > 0 && step < AuctionHouse.MAX_AUCTION_DURATION) {
            step++;
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        status.setOpen(false);
        status.setFinished(true);
        if (highestBidder == null) {
            System.out.println("\nAuction finished: Product not sold.\n");
        }
        else {
            Client winner = highestBidder.getClient();
            status.setWinner(winner);
            status.setBroker(highestBidder);

            winner.setWonAuctions(winner.getWonAuctions() + 1);
            highestBidder.setCommission(highestBidder.getCommission() + currentPrice * winner.getCommission());

            System.out.println("\nAuction " + ID + " finished: Sold to " + status.getWinner().getName() +
                                " for " + currentPrice + "\n");
        }
        Administrator admin = auctionHouse.getAdministrator();
        new Thread(new Delisting(admin, product)).start();
    }

}
