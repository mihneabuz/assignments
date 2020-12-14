import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.util.Scanner;

/**
 * clasa main deschide fisierle de intrare/iesire, parcurge rand cu rand,
 * genereaza graful si aplica comenzile necesare
 */
public class Main {
    public static void main(String[] args) {
        File inFile;
        File outFile;
        Scanner fileReader;
        FileWriter fileWriter;

        //Opening the files
        try {
            inFile = new File("map.in");
            fileReader = new Scanner(inFile);
        }
        catch (FileNotFoundException exception) {
            System.out.println("Can't open input file.");
            return;
        }
        try {
            outFile = new File("map.out");
            outFile.createNewFile();
            fileWriter = new FileWriter(outFile);
        }
        catch (java.io.IOException exception) {
            System.out.println("Can't create output file.");
            return;
        }

        //constructing the graph
        int no_roads = fileReader.nextInt();
        int no_nodes = fileReader.nextInt();
        Graph graph = new Graph(no_nodes);
        fileReader.nextLine();
        for (int i = 0; i < no_roads; i++) {
           String[] tokens = fileReader.nextLine().split(" ");
           int src_index = Integer.parseInt(tokens[0].substring(1));
           int dest_index = Integer.parseInt(tokens[1].substring(1));
           int cost = Integer.parseInt(tokens[2]);
           int limit = Integer.parseInt(tokens[3]);
           graph.addStreet(src_index, dest_index, cost, limit);
        }

        //reading the commands
        while(fileReader.hasNext()) {
            String[] tokens = fileReader.nextLine().split(" ");
            if (tokens[0].equals("accident") ||
                tokens[0].equals("trafic") ||
                tokens[0].equals("blocaj")) {
                int src_index = Integer.parseInt(tokens[1].substring(1));
                int dest_index = Integer.parseInt(tokens[2].substring(1));
                int cost = Integer.parseInt(tokens[3]);
                graph.addRestriction(tokens[0], src_index, dest_index, cost);
            }
            else {
                Vehicle veh = Vehicle.newVehicle(tokens[1].toCharArray()[0]);
                int src_index = Integer.parseInt(tokens[2].substring(1));
                int dest_index = Integer.parseInt(tokens[3].substring(1));
                try {
                    fileWriter.write(graph.drive(veh, src_index, dest_index) + "\n");
                }
                catch (java.io.IOException exception) {
                    System.out.println("Can't write to output file.");
                }
            }
        }
        try {
            fileWriter.close();
        }
        catch (java.io.IOException exception) {
            System.out.println("Can't close output file.");
        }

    }
}
