package CommandLineInterface;

import Main.Client;

import java.security.InvalidParameterException;

public class Deposit implements Command{
    @Override
    public void execute(Client client, String[] arguments) throws InvalidParameterException{
        if (arguments.length < 2)
            throw new InvalidParameterException("deposit requires 1 argument [amount]");
        client.setCredit(client.getCredit() + Double.parseDouble(arguments[1]));
        System.out.printf("Available credit: %.2f%n", client.getCredit());
    }
}
