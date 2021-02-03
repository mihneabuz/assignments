package Main;

public class LegalEntity extends Client {
    public enum Company {
        SRL,
        SA
    }

    private double capital;
    private Company company;

    public LegalEntity() {
        super();
    }

    public LegalEntity(int ID, String name, String address, double capital, String company) {
        super(ID, name, address);
        this.capital = capital;
        this.company = Company.valueOf(company);
    }

    @Override
    public double getCommission() {
        return (company == Company.SRL) ? 0.30 : 0.25;
    }

    public String toString() {
        return super.toString() + " from " + company.toString();
    }
}
