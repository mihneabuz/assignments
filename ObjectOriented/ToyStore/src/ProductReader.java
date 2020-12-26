import com.opencsv.CSVReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

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

    public Product readNext() throws IOException, InvalidProductException{
        String[] fields;
        try {
            fields = csvreader.readNext();
        }
        catch (Exception e) {
            throw new IOException("CSV reader failed");
        }
        try {
            String ID = fields[0];
            String name = fields[1];
            String manufacturerName = fields[2];
            double price;
            int quantity = 0;
            try {
                quantity = Integer.parseInt(fields[4].isEmpty() ? "0" : fields[4].split(" ")[0]);
                price = Currency.convert(fields[3].isEmpty() ? "€0" : fields[3], Currency.getDefaultCurrency());
            }
            catch (NumberFormatException e) {
                throw new InvalidProductException("Cannot parse price/quantity");
            }
            catch (NegativePriceException e) {
                price = 0f;
            }
            return new ProductBuilder()
                    .withID(ID)
                    .withName(name)
                    .withManufacturer(new Manufacturer(manufacturerName))
                    .withPrice(price)
                    .withQuantity(quantity)
                    .build();
        }
        catch (ArrayIndexOutOfBoundsException e) {
            throw new InvalidProductException("Not enough fields");
        }
    }
}
