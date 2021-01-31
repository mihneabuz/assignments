public class Listing implements Runnable {
    private Administrator admin;
    private Product product;

    public Listing(Administrator admin, Product product) {
        this.admin = admin;
        this.product = product;
    }

    @Override
    public void run() {
        admin.addProduct(product);
        //System.out.println("Added ID: " + product.getID());
    }
}
