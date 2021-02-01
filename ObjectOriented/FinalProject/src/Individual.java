public class Individual extends Client {
    private String birthDate;

    public Individual() {
        super();
    }

    public Individual(int ID, String name, String address, String birthDate) {
        super(ID, name, address);
        this.birthDate = birthDate;
    }
}
