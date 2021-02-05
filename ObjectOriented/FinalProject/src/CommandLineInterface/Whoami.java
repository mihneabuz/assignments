package CommandLineInterface;

import Main.Client;

import java.security.InvalidParameterException;

public class Whoami implements Command{
    @Override
    public void execute(Client client, String[] arguments) throws InvalidParameterException {
        assert client != null;
        System.out.println(client.toString() + "\nAuctions participated: " + client.getParticipatedAuctions() +
                "\nAuctions won: " + client.getWonAuctions() + "\nBalance: " + client.getCredit());
    }
}
