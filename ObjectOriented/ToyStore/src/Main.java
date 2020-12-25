public class Main {
    public static void main(String[] args) {
        Store store = Store.getInstance();
        try {
            store.init("Scule Bune", Currency.getCurrency("$"));
        }
        catch (CurrencyNotFoundException e) {
            System.out.println(e.toString());
            return;
        }

        store.readFrom("amazon_co-ecommerce_sample.csv");
        store.readProducts(5);

        try {
            store.changeCurrency(new Currency("LeiGrei", "G", 0.4f));
        }
        catch (Exception e) {
            System.out.println("nu");
        }

        System.out.println(store.toString());
        try {
            System.out.println(store.calculateTotal(store.getProductsByManufacturer("Hornby")));
        }
        catch (Exception e){
            System.out.println("nu");
        }

        store.applyDiscounts(new Discount("Reduceri Craciun", DiscountType.FIXED_DISCOUNT, 10));
        store.printInventory();
        System.out.println(store.toString());

        try {
            store.saveStore("data");
        }
        catch (Exception e) {
            System.out.println("nu mere ba " + e.toString());
        }


    }
}
