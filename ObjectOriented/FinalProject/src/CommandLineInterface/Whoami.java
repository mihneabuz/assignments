package CommandLineInterface;

import Main.Client;

import java.security.InvalidParameterException;

public class Whoami implements Command{
    @Override
    public void execute(Client client, String[] arguments) throws InvalidParameterException {
        System.out.println(client.toString() + "\nBalance: " + client.getCredit());
    }
}
