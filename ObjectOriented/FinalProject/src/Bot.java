import java.util.Random;

public class Bot extends Client {
    private int bidChance = new Random().nextInt(60);

    public Bot(int ID, String name, String address) {
        super(ID, name, address);
    }
}
