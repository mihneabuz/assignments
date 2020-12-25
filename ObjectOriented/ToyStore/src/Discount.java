import java.io.Serializable;
import java.time.LocalDateTime;

enum DiscountType {
    PERCENTAGE_DISCOUNT,
    FIXED_DISCOUNT
}

public class Discount {
    private String name;
    private DiscountType discountType;
    private double value;
    private LocalDateTime lastDateApplied;

    public Discount() {
    }

    public Discount(String name, DiscountType discountType, double value) {
        this.name = name;
        this.discountType = discountType;
        this.value = value;
        lastDateApplied = null;
    }

    public DiscountType getType() {
        return this.discountType;
    }

    public double getValue() {
        return value;
    }

    public LocalDateTime getLastDateApplied() {
        return lastDateApplied;
    }

    public void setAsAppliedNow() {
        lastDateApplied = LocalDateTime.now();
    }

    public String toString() {
        return "Discount " + name + " of type " + discountType + " for " + value;
    }
}
