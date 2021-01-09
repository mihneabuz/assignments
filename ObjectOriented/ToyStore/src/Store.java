import java.io.*;
import java.util.ArrayList;

public class Store {
    private static Store INSTANCE;
    private String name;
    private Currency currency;
    private final ArrayList<Product> products = new ArrayList<>();
    private final ArrayList<Manufacturer> manufacturers = new ArrayList<>();
    private final ArrayList<Discount> discounts = new ArrayList<>();
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

    public void setName(String name) {
        this.name = name;
    }

    public Currency getCurrency() {
        return currency;
    }

    /** initialize a ProductReader over file with filename **/
    public void readFrom(String filename) {
        try {
            productReader = new ProductReader(filename);
        }
        catch (FileNotFoundException e) {
            productReader = null;
            System.out.println(e.toString());
        }
    }

    /** reads all products from ProductReader **/
    public int readAllProducts() {
        int i = 0;
        while(readProduct()) {
            i++;
        }
        return i;
    }

    /** reads count products from ProductReader **/
    public void readProducts(int count) {
        for (int i = 0; i < count; i++)
            if(!readProduct())
                break;
    }

    /** reads 1 product from ProductReader **/
    public boolean readProduct() {
        if (productReader == null) {
            System.out.println("No file loaded");
            return false;
        }
        Product newProduct;
        try {
            newProduct = productReader.readNext();
        }
        catch (Exception e) {
            System.out.println(e.toString());
            return false;
        }
        if (newProduct == null)
            return false;
        try {
            addProduct(newProduct, true);
        }
        catch (DuplicateProductException e) {
            System.out.println(e.toString());
        }
        return true;
    }

    /** saves products to csv file **/
    public void saveToCsv(String filename) throws FileNotFoundException{
        PrintWriter output = new PrintWriter(new FileOutputStream(new File(filename)));
        for (Product product : products)
            output.print(product.compressed(currency.getSymbol()));
        output.close();
    }

    /** returns true if a product's ID is unique **/
    private boolean checkUniqueID(String newID) {
        for (Product product : products)
            if (product.getID().equals(newID))
                return false;
        return true;
    }

    /** returns true if a manufacturer's name is unique **/
    private boolean checkUniqueManufacturer(String manufacturerName) {
        for (Manufacturer manufacturer : manufacturers)
            if (manufacturer.getName().equals(manufacturerName))
                return false;
        return true;
    }

    /** returns manufacturer with the same name **/
    public Manufacturer findManufacturer(Manufacturer manufacturer) {
        return findManufacturer(manufacturer.getName());
    }

    /** returns manufacturer with name **/
    public Manufacturer findManufacturer(String manufacturerName) {
        for (Manufacturer manufacturer : manufacturers)
            if (manufacturer.getName().equals(manufacturerName))
                return manufacturer;
        return null;
    }

    /** returns products by manufacturer **/
    public ArrayList<Product> getProductsByManufacturer(Manufacturer manufacturer) throws ManufacturerNotFoundException {
        return getProductsByManufacturer(manufacturer.getName());
    }

    /** returns products by manufacturer with name **/
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

    /** adds a product to the store, converts it to store currency **/
    public void addProduct(Product product, boolean needsConversion) throws DuplicateProductException {
        if (!checkUniqueID(product.getID()))
            throw new DuplicateProductException("Duplicate product found. ID not unique");
        try {
            addManufacturer(product.getManufacturer());
        }
        catch (DuplicateManufacturerException e) {
            product.setManufacturer(findManufacturer(product.getManufacturer()));
        }
        product.getManufacturer().incCount();
        if (needsConversion)
            try {
                product.setPrice(Currency.convert(product.getPrice(), Currency.getDefaultCurrency(), currency));
            }
            catch (NegativePriceException e) {
                product.setPrice(0f);
            }
        this.products.add(product);
    }

    /** returns products with ID **/
    public Product getProduct(String ID) throws ProductNotFoundException{
        for (Product product : products) {
            if (product.getID().equals(ID)) {
                return product;
            }
        }
        throw new ProductNotFoundException("Product " + ID + " not found.");
    }

    /** adds a new manufacturer to the store **/
    public void addManufacturer(Manufacturer manufacturer) throws DuplicateManufacturerException {
        if (!checkUniqueManufacturer(manufacturer.getName()))
            throw new DuplicateManufacturerException("Duplicate manufacturer found. Name is not unique");
        this.manufacturers.add(manufacturer);
    }

    /** changes the currency of the store to Currency with name **/
    public void changeCurrency(String name) throws CurrencyNotFoundException{
        Currency newCurrency = Currency.getCurrency(name);
        changeCurrency(newCurrency);
    }

    /** changes the currency of the store to newCurrency **/
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

    /** adds a discount to the store **/
    public void addDiscount(Discount discount) {
        discounts.add(discount);
    }

    /** returns discount with type and value **/
    public Discount getDiscount(DiscountType type, double value) throws DiscountNotFoundException {
        for (Discount discount : discounts)
            if (discount.getType().equals(type) && discount.getValue() == value)
                return discount;
        throw new DiscountNotFoundException("Discount of " + value + " " + type.toString() + " not found.");
    }

    /** applies discount to all products **/
    public void applyDiscounts(Discount discount) {
        applyDiscounts(discount, products);
    }

    /** applies discount to every product in list products **/
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

    /** returns total value of all products in store **/
    public double calculateTotal() {
        return calculateTotal(products);
    }

    /** returns total value of every product in list products **/
    public double calculateTotal(ArrayList<Product> products) {
        double total = 0f;
        for (Product product : products)
            total += product.getPrice() * product.getQuantity();
        return total;
    }

    /** saves state to binary file **/
    public void saveState(String filename) throws IOException {
        BinaryOutputStream writer = new BinaryOutputStream(new BufferedOutputStream(
                                        new FileOutputStream(new File(filename))));
        writer.write(name);
        currency.writeBin(writer);
        writer.write(products.size());
        for (Product product : products)
            product.writeBin(writer);
        writer.close();
    }

    /** loads state from binary file. Doesn't reset reader. **/
    public void loadState(String filename) throws IOException {
        BinaryInputStream reader = new BinaryInputStream(new BufferedInputStream(
                new FileInputStream(new File(filename))));
        products.clear();
        manufacturers.clear();
        discounts.clear();
        this.setName(reader.readString());
        String currencyName = reader.readString();
        String currencySymbol = reader.readString();
        double currencyParity = reader.readDouble();
        changeCurrency(new Currency(currencyName, currencySymbol, currencyParity));
        int nrProducts = reader.readInt();
        for (int i = 0; i < nrProducts; i++) {
            String productID = reader.readString();
            String productName = reader.readString();
            String productManufacturerName = reader.readString();
            double productPrice = reader.readDouble();
            int productQuantity = reader.readInt();
            try {
                addProduct(new ProductBuilder()
                        .withID(productID)
                        .withName(productName)
                        .withManufacturer(new Manufacturer(productManufacturerName))
                        .withPrice(productPrice)
                        .withQuantity(productQuantity)
                        .build()
                        , false);
            }
            catch (DuplicateProductException e) { //this should never happen
            }
        }
        reader.close();
    }

    /** prints all store inventory **/
    public void printInventory() {
        printInventory(products);
    }

    /** prints every product in list products **/
    public void printInventory(ArrayList<Product> products) {
        System.out.println("Products: " + products.size());
        for (Product product : products) {
            System.out.println(product.toString());
        }
    }

    /** prints all store manufacturers **/
    public void printManufacturerList() {
        System.out.println("Manufacturers: " + manufacturers.size());
        for (Manufacturer manufacturer : manufacturers) {
            System.out.println(manufacturer.toString());
        }
    }

    /** prints all store discounts **/
    public void printDiscounts() {
        System.out.println("Discounts: " + discounts.size());
        for (Discount discount : discounts)
            System.out.println(discount.toString());
    }

    public String toString() {
        return "Store " + name + "\n" + currency.toString() + "\nTotal: " + String.format("%.2f", calculateTotal());
    }
}
