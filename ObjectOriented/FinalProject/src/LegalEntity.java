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

    public LegalEntity(int ID, String name, String address, int participatedAuctions, int wonAuctions, double capital, Company company) {
        super(ID, name, address, participatedAuctions, wonAuctions);
        this.capital = capital;
        this.company = company;
    }
}
