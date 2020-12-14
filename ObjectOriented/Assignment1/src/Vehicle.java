/**
 * clasa Vehicle definete tipul vehicului care parcurge graful
 *
 * este mostenit de clasele Bicycle, Motorbike, Car si Truck
 *
 */
public class Vehicle {
    protected int limit;
    protected int cost;

    public Vehicle() {
    }

    /**
     * Creaza un nou vehicul de tipul cerut
     *
     * @param c tipul vehiculului
     * @return un obiect de tipul cerut
     */
    static Vehicle newVehicle(char c) {
        Vehicle aux = null;
        if (c == 'b') {
            aux = new Bicycle();
        }
        else if (c == 'm') {
            aux = new Motorbike();
        }
        else if (c == 'a') {
            aux = new Car();
        }
        else if (c == 'c') {
            aux = new Truck();
        }
        return aux;
    }

    public int getLimit() {
        return limit;
    }

    public int getCost() {
        return cost;
    }
}
