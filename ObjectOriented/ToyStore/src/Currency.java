import java.util.ArrayList;

public class Currency {
    private static final ArrayList<Currency> currencies = new ArrayList<>(4);
    private String name;
    private String symbol;
    private double parityToEur;

    static {
        currencies.add(new Currency("Euro", "€", 1.0f));
        currencies.add(new Currency("Pound", "£", 1.09f));
        currencies.add(new Currency("Dollar", "$", 0.82f));
        currencies.add(new Currency("Leu", "L", 0.2f));
    }

    public static double convert(String string, Currency newCurrency) throws NegativePriceException {
        String symbol = string.substring(0, 1);
        double value = Double.parseDouble(string.substring(1));
        Currency oldCurrency;
        try {
             oldCurrency = getCurrency(symbol);
        }
        catch (CurrencyNotFoundException e) {
            System.out.println(e.toString());
            return 0f;
        }
        return convert(value, oldCurrency, newCurrency);
    }

    public static double convert(double value, Currency oldCurrency, Currency newCurrency) throws NegativePriceException {
        value = value * oldCurrency.getParityToEur() / newCurrency.getParityToEur();
        if (value < 0)
            throw new NegativePriceException(" ");
        return value;
    }

    public static Currency getCurrency(String symbol) throws CurrencyNotFoundException {
        for (Currency currency : currencies) {
            if (currency.getSymbol().equals(symbol)) {
                return currency;
            }
        }
        throw new CurrencyNotFoundException("Currency: '" + symbol + "' not found");
    }

    public Currency() {
    }

    public Currency(String name, String symbol, double parityToEur) {
        this.name = name;
        this.symbol = symbol;
        this.parityToEur = parityToEur;
        currencies.add(this);
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
        return "Currency " + name + '(' + symbol + ')';
    }
}
