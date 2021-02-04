package Main;

public class Individual extends Client {
    private String birthDate;

    public Individual() {
        super();
    }

    public Individual(String name, String address, String birthDate) {
        super(name, address);
        this.birthDate = birthDate;
    }

    @Override
    public double getCommission() {
        return 0.15;
    }

    public String toString() {
        return super.toString() + " born in " + birthDate;
    }
}
