package CommandLineInterface;

import Main.Client;

import java.security.InvalidParameterException;

public class Dismiss implements Command{
    @Override
    public void execute(Client client, String[] arguments) throws InvalidParameterException {
        assert client != null;
        if (!client.hasBroker())
            System.out.println("You don't currently have a broker");
        else {
            System.out.println("Broker " + client.getBroker().getName() + " dismissed!");
            client.dismissBroker();
        }
    }
}
