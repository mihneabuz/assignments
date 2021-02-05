package CommandLineInterface;

import Main.AuctionHouse;
import Main.Client;

import java.security.InvalidParameterException;

public class Request implements Command{
    @Override
    public void execute(Client client, String[] arguments) throws InvalidParameterException {
        assert client != null;
        client.requestBroker(AuctionHouse.getINSTANCE());
        System.out.println("Assigned Broker " + client.getBroker().getName());
    }
}
