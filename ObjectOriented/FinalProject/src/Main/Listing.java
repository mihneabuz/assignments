package Main;

public class Listing implements Runnable {
    private final Administrator admin;
    private final Product product;

    public Listing(Administrator admin, Product product) {
        this.admin = admin;
        this.product = product;
    }

    @Override
    public void run() {
        admin.addProduct(product);
        System.err.println("-> Added product with ID: " + product.getID());
    }
}
