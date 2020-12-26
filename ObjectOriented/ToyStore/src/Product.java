import java.io.IOException;
import java.time.format.DateTimeFormatter;

public class Product {
    private String uniqueID;
    private String name;
    private Manufacturer manufacturer;
    private double price;
    private int quantity;
    private Discount discount;

    public Product() {
    }

    public Product(String uniqueID, String name, Manufacturer manufacturer, double price, int quantity, Discount discount) {
        this.uniqueID = uniqueID;
        this.name = name;
        this.manufacturer = manufacturer;
        this.price = price;
        this.quantity = quantity;
        this.discount = discount;
    }

    public String getID() {
        return uniqueID;
    }

    public double getPrice() {
        return price;
    }

    public Discount getDiscount() {
        return discount;
    }

    public Manufacturer getManufacturer() {
        return manufacturer;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setUniqueID(String uniqueID) {
        this.uniqueID = uniqueID;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setManufacturer(Manufacturer manufacturer) {
        this.manufacturer = manufacturer;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public void applyDiscount(Discount discount) throws NegativePriceException {
        this.discount = discount;
        if (discount.getType() == DiscountType.PERCENTAGE_DISCOUNT) {
            price = price * (100 - discount.getValue()) / 100;
        }
        else if (discount.getType() == DiscountType.FIXED_DISCOUNT) {
            price = price - discount.getValue();
        }
        if (price <= 0f) {
            price = 0f;
            throw new NegativePriceException("Negative price after discount");
        }
    }

    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("ID: " + uniqueID +
                             "\nName: " + name +
                             "\nManufacturer: " + manufacturer.getName() + '\n');
        if (price != 0) {
            stringBuilder.append("Price: " + String.format("%.2f", price) + '\n');
            if (discount != null && discount.getLastDateApplied() != null)
                stringBuilder.append("Discounted on " +
                        discount.getLastDateApplied().format(
                                DateTimeFormatter.ofPattern("dd-MM-yyyy HH:mm:ss")) + '\n');
        }
        else
            stringBuilder.append("Price not Available\n");
        if (quantity != 0)
            stringBuilder.append("Stock: " + quantity + '\n');
        else
            stringBuilder.append("Out of Stock!\n");
        return stringBuilder.toString();
    }

    public void writeBin(BinaryOutputStream writer) throws IOException {
        writer.write(uniqueID);
        writer.write(name);
        writer.write(manufacturer.getName());
        writer.write(price);
        writer.write(quantity);
    }
}
