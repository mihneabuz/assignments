import java.io.*;
import java.util.ArrayList;

public class Store {
    private static Store INSTANCE;
    private String name;
    private Currency currency;
    private final ArrayList<Product> products = new ArrayList<>();
    private final ArrayList<Manufacturer> manufacturers = new ArrayList<>();
    private ProductReader productReader;

    private Store() {
    }

    public static Store getInstance() {
        if (INSTANCE == null)
            INSTANCE = new Store();
        return INSTANCE;
    }

    public void init(String name, Currency currency) {
        this.name = name;
        this.currency = currency;
    }

    public void readFrom(String filename) {
        try {
            productReader = new ProductReader(filename);
        }
        catch (FileNotFoundException e) {
            productReader = null;
            System.out.println(e.toString());
        }
    }

    public void readProducts(int count) {
        for (int i = 0; i < count; i++)
            readProducts();
    }

    public void readProducts() {
        if (productReader == null) {
            System.out.println("No file loaded");
            return;
        }
        Product newProduct;
        try {
            newProduct = productReader.readNext();
        }
        catch (Exception e) {
            System.out.println(e.toString());
            return;
        }
        try {
            addProduct(newProduct);
        }
        catch (DuplicateProductException e) {
            System.out.println(e.toString());
        }
    }

    private boolean checkUniqueID(String newID) {
        for (Product product : products)
            if (product.getID().equals(newID))
                return false;
        return true;
    }

    private boolean checkUniqueManufacturer(String manufacturerName) {
        for (Manufacturer manufacturer : manufacturers)
            if (manufacturer.getName().equals(manufacturerName))
                return false;
        return true;
    }

    public Manufacturer findManufacturer(Manufacturer manufacturer) {
        return findManufacturer(manufacturer.getName());
    }

    public Manufacturer findManufacturer(String manufacturerName) {
        for (Manufacturer manufacturer : manufacturers)
            if (manufacturer.getName().equals(manufacturerName))
                return manufacturer;
        return null;
    }

    public ArrayList<Product> getProductsByManufacturer(Manufacturer manufacturer) throws ManufacturerNotFoundException {
        return getProductsByManufacturer(manufacturer.getName());
    }

    public ArrayList<Product> getProductsByManufacturer(String manufacturerName) throws ManufacturerNotFoundException {
        boolean found = false;
        for (Manufacturer manufacturer : manufacturers)
            if (manufacturer.getName().equals(manufacturerName)) {
                found = true;
                break;
            }
        if (!found)
            throw new ManufacturerNotFoundException("Manufacturer \"" + manufacturerName + "\" not found");

        ArrayList<Product> aux = new ArrayList<>();
        for (Product product : products)
            if (product.getManufacturer().getName().equals(manufacturerName))
                aux.add(product);
        return aux;
    }

    public void addProduct(Product product) throws DuplicateProductException {
        if (!checkUniqueID(product.getID()))
            throw new DuplicateProductException("Duplicate product found. ID not unique");
        try {
            addManufacturer(product.getManufacturer());
        }
        catch (DuplicateManufacturerException e) {
            product.setManufacturer(findManufacturer(product.getManufacturer()));
        }
        product.getManufacturer().incCount();
        try {
            product.setPrice(Currency.convert(product.getPrice(), Currency.getDefaultCurrency(), currency));
        }
        catch (NegativePriceException e) {
            product.setPrice(0f);
        }
        this.products.add(product);
    }

    public void addManufacturer(Manufacturer manufacturer) throws DuplicateManufacturerException {
        if (!checkUniqueManufacturer(manufacturer.getName()))
            throw new DuplicateManufacturerException("Duplicate manufacturer found. Name is not unique");
        this.manufacturers.add(manufacturer);
    }

    public void changeCurrency(String symbol) throws CurrencyNotFoundException{
        Currency newCurrency = Currency.getCurrency(symbol);
        changeCurrency(newCurrency);
    }

    public void changeCurrency(Currency newCurrency) {
        for (Product product : products) {
            try {
                product.setPrice(Currency.convert(product.getPrice(), currency, newCurrency));
            }
            catch (NegativePriceException e) {
                product.setPrice(0f);
            }
        }
        currency = newCurrency;
    }

    public void applyDiscounts(Discount discount) {
        applyDiscounts(discount, products);
    }

    public void applyDiscounts(Discount discount, ArrayList<Product> products) {
        for (Product product : products) {
            try {
                product.applyDiscount(discount);
            }
            catch (NegativePriceException e) {
                System.out.println(e.toString());
            }
        }
        discount.setAsAppliedNow();
    }

    public double calculateTotal() {
        return calculateTotal(products);
    }

    public double calculateTotal(ArrayList<Product> products) {
        double total = 0f;
        for (Product product : products)
            total += product.getPrice() * product.getQuantity();
        return total;
    }

    public void saveStore(String filename) throws IOException {
        BinaryOutputStream writer = new BinaryOutputStream(new BufferedOutputStream(
                                        new FileOutputStream(new File(filename))));
        writer.write(name);
        currency.writeBin(writer);
        writer.write(products.size());
        for (Product product : products)
            product.writeBin(writer);
        writer.close();
    }

    public void printInventory() {
        printInventory(products);
    }

    public void printInventory(ArrayList<Product> products) {
        System.out.println("Products: " + products.size());
        for (Product product : products) {
            System.out.println(product.toString());
        }
    }

    public void printManufacturerList() {
        System.out.println("Manufacturers: " + manufacturers.size());
        for (Manufacturer manufacturer : manufacturers) {
            System.out.println(manufacturer.toString());
        }
    }

    public String toString() {
        return "Store " + name + "\n" + currency.toString() + "\nTotal: " + String.format("%.2f", calculateTotal());
    }
}