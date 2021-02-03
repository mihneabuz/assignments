package Main;

public class AuctionStatus {
    private boolean open;
    private boolean finished;
    private Client winner;
    private Broker broker;

    public AuctionStatus() {
        this.open = false;
        this.finished = false;
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

    public boolean isFinished() {
        return finished;
    }

    public void setFinished(boolean finished) {
        this.finished = finished;
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
