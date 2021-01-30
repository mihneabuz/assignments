public class Individual extends Client {
    private String birthDate;

    public Individual() {
        super();
    }

    public Individual(int ID, String name, String address, int participatedAuctions, int wonAuctions, String birthDate) {
        super(ID, name, address, participatedAuctions, wonAuctions);
        this.birthDate = birthDate;
    }
}
