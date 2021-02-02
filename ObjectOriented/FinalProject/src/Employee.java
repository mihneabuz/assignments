public class Employee {
    private int ID;
    private String name;

    public Employee() {
    }

    public Employee(int ID, String name) {
        this.ID = ID;
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public int getID() {
        return ID;
    }

    public String toString() {
        return this.name;
    }
}
