/**
 * clasa ce defineste un ambuteiaj
 */
public class Restriction {
    private String type;
    private int cost;

    public Restriction() {
    }

    public Restriction(String type, int cost) {
        this.type = type;
        this.cost = cost;
    }

    public int getCost() {
        return this.cost;
    }

    public String getType() {
        return this.type;
    }
}
