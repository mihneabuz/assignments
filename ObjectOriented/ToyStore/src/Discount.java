import java.time.LocalDateTime;

public class Discount {
    private String name;
    private DiscountType discountType;
    private double value;
    private LocalDateTime lastDateApplied = null;

    public Discount() {
    }

    public Discount(String name, DiscountType discountType, double value) {
        this.name = name;
        this.discountType = discountType;
        this.value = value;
    }

    public DiscountType getType() {
        return this.discountType;
    }

    public double getValue() {
        return value;
    }

    public void setAsAppliedNow() {
        lastDateApplied = LocalDateTime.now();
    }

    public String toString() {
        return "Discount " + name + " of type " + discountType + " for " + value;
    }
}
