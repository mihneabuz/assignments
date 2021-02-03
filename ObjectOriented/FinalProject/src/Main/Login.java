package Main;

import CommandLineInterface.Cli;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Login implements Runnable {
    private AuctionHouse auctionHouse;
    private Client client;

    public Login(AuctionHouse auctionHouse) {
        this.auctionHouse = auctionHouse;
    }

    @Override
    public void run() {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        int choice = 0;
        String name = null;
        String address = null;

        System.out.println("Welcome to Auctioneer 2.0\n\nWould you like to login as:\n1) Individual\n2) Legal Entity");
        try {
            choice = Integer.parseInt(reader.readLine());
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }

        System.out.println("Please input your name:");
        try {
            name = reader.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Please input your address:");
        try {
            address = reader.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (choice == 1) {
            System.out.println("Please input your birthdate:");
            String birthdate = null;
            try {
                birthdate = reader.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
            client = new Individual(0, name, address, birthdate);
        }
        else {
            System.out.println("Please input your Company type:");
            String companyType = null;
            try {
                companyType = reader.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
            System.out.println("Please input your capital:");
            double capital = 0;
            try {
                capital = Double.parseDouble(reader.readLine());
            } catch (IOException e) {
                e.printStackTrace();
            }
            client = new LegalEntity(0, name, address, capital, companyType);
        }

        new Register(auctionHouse, client).run();
        System.out.println("Registered user " + client.toString() + "\n\nWelcome!");

        Cli cli = new Cli(client, reader);
        try {
            cli.run();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
