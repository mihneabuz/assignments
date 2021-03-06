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

    public LegalEntity(String name, String address, double capital, String company) {
        super(name, address);
        this.capital = capital;
        this.company = Company.valueOf(company);
        setCredit(capital);
    }

    public double getCapital() {
        return capital;
    }

    public Company getCompany() {
        return company;
    }

    @Override
    public double getCommission() {
        return (company == Company.SRL) ? 0.30 : 0.25;
    }

    public String toString() {
        return super.toString() + ", " + company.toString() + " Company";
    }
}
