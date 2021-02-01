public class AuctionStatus {
    private boolean open;
    private Client winner;
    private Broker broker;

    public AuctionStatus() {
        this.open = true;
    }

    public boolean isOpen() {
        return open;
    }

    public Client getWinner() {
        return winner;
    }

    public Broker getBroker() {
        return broker;
    }

    public void setOpen(boolean open) {
        this.open = open;
    }

    public void setWinner(Client winner) {
        this.winner = winner;
    }

    public void setBroker(Broker broker) {
        this.broker = broker;
    }
}
