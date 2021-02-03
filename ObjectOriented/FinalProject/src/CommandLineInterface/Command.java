package CommandLineInterface;

import Main.Client;

import java.security.InvalidParameterException;

public interface Command {
    void execute(Client client, String[] arguments) throws InvalidParameterException;
}
