public class Delisting implements Runnable {
    private Administrator admin;
    private Product product;

    public Delisting(Administrator admin, Product product) {
        this.admin = admin;
        this.product = product;
    }

    @Override
    public void run() {
        admin.removeProduct(product);
        //System.out.println("Removed ID: " + product.getID());
    }
}