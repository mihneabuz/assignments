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

        System.out.println(store.toString());

        store.readFrom("amazon_co-ecommerce_sample.csv");
        store.readProducts(5);
        store.printInventory();
        store.printManufacturerList();

        try {
            store.changeCurrency(new Currency("LeiGrei", "G", 0.4f));
        }
        catch (Exception e) {

        }

        System.out.println();
        System.out.println();
        System.out.println(store.toString());
        store.printInventory();
        store.printManufacturerList();

        System.out.println(store.toString());
        try {
            System.out.println(store.calculateTotal(store.getProductsByManufacturer("Hornby")));
        }
        catch (Exception e){

        }

    }
}
