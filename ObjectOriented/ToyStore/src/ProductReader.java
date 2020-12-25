import com.opencsv.CSVReader;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class ProductReader {
    private final CSVReader csvreader;

    public ProductReader(String filename) throws FileNotFoundException{
        this(new File(filename));
    }

    public ProductReader(File csv) throws FileNotFoundException {
        FileReader fileReader;
        fileReader = new FileReader(csv);
        csvreader = new CSVReader(fileReader);
        try {
            csvreader.readNext();
        }
        catch (Exception e) {
            throw new FileNotFoundException("File not found or is empty");
        }
    }

    public ArrayList<String> readNext() {
        String[] fields;
        try {
            fields = csvreader.readNext();
        }
        catch (Exception e) {
            return null;
        }
        try {
            ArrayList<String> productFields = new ArrayList<>(5);
            productFields.add(fields[0]);
            productFields.add(fields[1]);
            productFields.add(fields[2]);
            productFields.add(fields[3].isEmpty() ? "0" : fields[3]);
            productFields.add(fields[4].isEmpty() ? "0" : fields[4].split(" ")[0]);
            try {
                Integer.parseInt(productFields.get(4));
                Double.parseDouble(productFields.get(3).substring(1));
            }
            catch (NumberFormatException e) {
                return null;
            }
            return productFields;
        }
        catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }

    public void close() {
        try {
            csvreader.close();
        }
        catch (IOException e){
        }
    }

}
