import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Operator operator = new Operator();
        Store store = Store.getInstance();
        try {
            store.init("Cool Toys (tm)", Currency.getCurrency("EUR"));
        }
        catch (CurrencyNotFoundException e) {
            System.out.println(e.toString());
            return;
        }
        operator.setStore(store);
        Scanner scanner = new Scanner(System.in);
        String command = scanner.nextLine();
        while(operator.command(command)) {
            command = scanner.nextLine();
        }
    }
}
