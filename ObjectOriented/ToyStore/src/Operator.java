import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Operator {
    private Store store;

    public Operator(Store store) {
        this.store = store;
    }

    public Operator() {
    }

    public Store getStore() {
        return store;
    }

    public void setStore(Store store) {
        this.store = store;
    }

    public boolean command(String line) {
        String cmd;
        ArrayList<String> arguments = new ArrayList<>();
        Matcher m = Pattern.compile("([^\"]\\S*|\".+?\")\\s*").matcher(line);
        if (m.find())
            cmd = m.group(1);
        else {
            System.out.println("input a command");
            return true;
        }
        while(m.find())
            arguments.add(m.group(1).replace("\"", ""));

        switch (cmd) {
            case "loadcsv":
                if (arguments.size() < 1) {
                    System.out.println("not enough arguments");
                    return true;
                }
                store.readFrom(arguments.get(0));
                break;
            case "storeinfo":
                System.out.println(store.toString());
                break;
            case "readproducts":
                if (arguments.get(0).equals("all"))
                    System.out.println(store.readAllProducts() + " products read.");
                else
                    store.readProducts(Integer.parseInt(arguments.get(0)));
                break;
            case "savecsv":
                if (arguments.size() < 1) {
                    System.out.println("not enough arguments");
                    return true;
                }
                try {
                    store.saveToCsv(arguments.get(0));
                } catch (FileNotFoundException e) {
                    System.out.println(e.toString());
                }
                break;
            case "getstorecurrency":
                System.out.println(store.getCurrency().toString());
                break;
            case "setstorecurrency":
                if (arguments.size() < 1) {
                    System.out.println("not enough arguments");
                    return true;
                }
                try {
                    store.changeCurrency(Currency.getCurrency(arguments.get(0)));
                } catch (CurrencyNotFoundException e) {
                    System.out.println(e.toString());
                }
                break;
            case "listproducts":
                store.printInventory();
                break;
            case "showproduct":
                if (arguments.size() < 1) {
                    System.out.println("not enough arguments");
                    return true;
                }
                try {
                    System.out.println(store.getProduct(arguments.get(0)).toString());
                } catch (ProductNotFoundException e) {
                    System.out.println(e.toString());
                }
                break;
            case "listmanufacturers":
                store.printManufacturerList();
                break;
            case "listproductsbymanufacturer":
                if (arguments.size() < 1) {
                    System.out.println("not enough arguments");
                    return true;
                }
                try {
                    store.printInventory(store.getProductsByManufacturer(arguments.get(0)));
                } catch (ManufacturerNotFoundException e) {
                    System.out.println(e.toString());
                }
                break;
            case "calculatetotal":
                if (arguments.size() == 0) {
                    System.out.printf("Total: %.2f%n", store.calculateTotal());
                } else {
                    ArrayList<Product> products = new ArrayList<>();
                    for (String ID : arguments) {
                        try {
                            products.add(store.getProduct(ID));
                        } catch (ProductNotFoundException e) {
                            System.out.println(e.toString());
                        }
                    }
                    System.out.printf("Total: %.2f%n", store.calculateTotal(products));
                }
                break;
            case "addcurrency":
                if (arguments.size() < 3) {
                    System.out.println("not enough arguments");
                    return true;
                }
                try {
                    Currency.getCurrency(arguments.get(0)).updateParity(Double.parseDouble(arguments.get(2)));
                } catch (CurrencyNotFoundException e) {
                    new Currency(arguments.get(0), arguments.get(1), Double.parseDouble(arguments.get(2)));
                }
                break;
            case "listcurrencies":
                Currency.printCurrencies();
                break;
            case "updateparity":
                if (arguments.size() < 1) {
                    System.out.println("not enough arguments");
                    return true;
                }
                Currency toUpdate;
                try {
                    toUpdate = Currency.getCurrency(arguments.get(0));
                    toUpdate.updateParity(Double.parseDouble(arguments.get(1)));
                } catch (CurrencyNotFoundException e) {
                    System.out.println(e.toString());
                }
                break;
            case "listdiscounts":
                store.printDiscounts();
                break;
            case "adddiscount":
                if (arguments.size() < 3) {
                    System.out.println("not enough arguments");
                    return true;
                }
                store.addDiscount(new Discount(arguments.get(2), DiscountType.valueOf(arguments.get(0) + "_DISCOUNT"), Double.parseDouble(arguments.get(1))));
                break;
            case "applydiscount":
                if (arguments.size() < 2) {
                    System.out.println("not enough arguments");
                    return true;
                }
                try {
                    store.applyDiscounts(store.getDiscount(DiscountType.valueOf(arguments.get(0) + "_DISCOUNT"), Integer.parseInt(arguments.get(1))));
                } catch (DiscountNotFoundException e) {
                    System.out.println(e.toString());
                }
                break;
            case "savestate":
                try {
                    store.saveState("data.bin");
                } catch (IOException e) {
                    System.out.println(e);
                }
                break;
            case "loadstate":
                try {
                    store.loadState("data.bin");
                } catch (Exception e) {
                    System.out.println(e);
                }
                break;
            case "exit":
                return false;
            case "quit":
                return false;
            default:
                System.out.println("command not found");
                break;
        }

        return true;
    }
}
