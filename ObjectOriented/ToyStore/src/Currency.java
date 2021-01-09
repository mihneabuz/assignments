import java.io.IOException;
import java.util.ArrayList;

public class Currency {
    private static final ArrayList<Currency> currencies = new ArrayList<>(4);
    private String name;
    private String symbol;
    private double parityToEur;

    /* initial entries of the static Currency list, first entry considered default Currency */
    static {
        new Currency("EUR", "€", 1.0f);
        new Currency("GBP", "£", 1.09f);
        new Currency("USD", "$", 0.82f);
        new Currency("RON", "L", 0.2f);
    }

    /** converts a string of type "(symbol)(value)" into a value of newCurrency **/
    public static double convert(String string, Currency newCurrency) throws NegativePriceException, NumberFormatException {
        String symbol = string.substring(0, 1);
        double value = Double.parseDouble(string.substring(1));
        Currency oldCurrency;
        try {
             oldCurrency = getCurrencyBySymbol(symbol);
        }
        catch (CurrencyNotFoundException e) {
            System.out.println(e.toString());
            return 0f;
        }
        return convert(value, oldCurrency, newCurrency);
    }

    /** converts value from oldCurrency to newCurrency **/
    public static double convert(double value, Currency oldCurrency, Currency newCurrency) throws NegativePriceException {
        value = value * oldCurrency.getParityToEur() / newCurrency.getParityToEur();
        if (value < 0)
            throw new NegativePriceException(" ");
        return value;
    }

    /** returns Currency with name **/
    public static Currency getCurrency(String name) throws CurrencyNotFoundException {
        for (Currency currency : currencies) {
            if (currency.getName().equals(name)) {
                return currency;
            }
        }
        throw new CurrencyNotFoundException("Currency: '" + name + "' not found");
    }

    /** returns Currency with symbol **/
    public static Currency getCurrencyBySymbol(String symbol) throws CurrencyNotFoundException {
        for (Currency currency : currencies) {
            if (currency.getSymbol().equals(symbol)) {
                return currency;
            }
        }
        throw new CurrencyNotFoundException("Currency: '" + symbol + "' not found");
    }

    /** adds a new Currency to the static list. If it exists already, update the parity **/
    public static void addCurrency(Currency currency) {
        try {
            Currency old = getCurrency(currency.getName());
            old.updateParity(currency.getParityToEur());
        }
        catch (CurrencyNotFoundException e) {
            currencies.add(currency);
        }
    }

    /** prints the list of currencies **/
    public static void printCurrencies() {
        for (Currency currency : currencies) {
            System.out.println(currency.toString());
        }
    }

    /** returns default currency **/
    public static Currency getDefaultCurrency() {
        return currencies.get(0);
    }

    public Currency() {
    }

    public Currency(String name, String symbol, double parityToEur) {
        this.name = name;
        this.symbol = symbol;
        this.parityToEur = parityToEur;
        addCurrency(this);
    }

    public void updateParity(double parityToEur) {
        this.parityToEur = parityToEur;
    }

    public String getName() {
        return name;
    }

    public String getSymbol() {
        return symbol;
    }

    public double getParityToEur() {
        return parityToEur;
    }

    public String toString() {
        return "Currency " + name + '(' + symbol + ") " + String.format("%.2f", parityToEur);
    }

    /** writes information to binary writer **/
    public void writeBin(BinaryOutputStream writer) throws IOException {
        writer.write(name);
        writer.write(symbol);
        writer.write(parityToEur);
    }
}
