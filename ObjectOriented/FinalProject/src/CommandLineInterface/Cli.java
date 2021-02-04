package CommandLineInterface;
import Main.AuctionHouse;
import Main.Client;
import Main.Simulation;

import java.io.BufferedReader;
import java.io.IOException;
import java.security.InvalidParameterException;
import java.util.HashMap;

public class Cli {
    private static HashMap<String, Command> commands = new HashMap<>();
    static {
        commands.put("products", new ListProducts());
        commands.put("whoami", new Whoami());
        commands.put("deposit", new Deposit());
        commands.put("request", new Request());
        commands.put("auctions", new ListAuctions());
        commands.put("enter", new EnterAuction());
    }

    private final AuctionHouse auctionHouse = AuctionHouse.getINSTANCE();
    private final Client client;
    private final BufferedReader reader;

    public Cli(Client client, BufferedReader reader) {
        this.client = client;
        this.reader = reader;
    }

    public void run() throws IOException {
        System.out.println("Commands: whoami | deposit | products | auctions | request | enter | quit");
        String cmd = reader.readLine();
        while (!cmd.equals("quit")) {
            String[] args = cmd.split(" ");
            try {
                Command command = commands.get(args[0]);
                if (command != null)
                    command.execute(client, args);
                else
                    System.out.println("Invalid Command");
            }
            catch (InvalidParameterException e) {
                System.out.println(e.getMessage());
            }
            cmd = reader.readLine();
        }
        Simulation.end();
    }
}
