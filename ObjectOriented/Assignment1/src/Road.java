/**
 * clasa ce defineste un drum
 *
 * atributele sunt: un nod sursa si un nod destinatie, costul, limita de gabarit,
 * si un cost total al ambuteiajelor
 */
class Road {
    final private Node src;
    final private Node dest;
    final private int cost;
    private int cost_environmental;
    final private int limit;

    public Road() {
        this.src = null;
        this.dest = null;
        this.cost = 0;
        this.limit = 0;
    }

    public Road(Node src, Node dest, int cost, int limit) {
        this.src = src;
        this.dest = dest;
        this.cost = cost;
        this.limit = limit;
        this.cost_environmental = 0;
    }

    /**
     * verifica daca un vehicul poate circula pe strada (limita de gabarit)
     *
     * @param veh: vehiculul
     * @return 1 daca vehiculul poate circula, 0 altfel
     */
    public boolean canDrive(Vehicle veh) {
        if (veh.getLimit() > this.limit)
            return false;
        return true;
    }

    /**
     * calculeaza costul drumui pentru un vehicul
     *
     * @param veh: vechiculul
     * @return costul drumului pentru acel vehicul
     */
    public int computeCost(Vehicle veh) {
        return (this.cost * veh.getCost()) + this.cost_environmental;
    }

    /**
     * adauga costul unui ambuteiaj
     *
     * @param restr: ambuteiajul
     */
    public void restrict(Restriction restr) {
        this.cost_environmental += restr.getCost();
    }

    public int getDest() {
        if (this.dest == null)
            return -1;
        return this.dest.index;
    }

    public int getSource() {
        if (this.src == null)
            return -1;
        return this.src.index;
    }

    /**
     * pentru debugging
     */
    public void print() {
        System.out.println(this.src.index + " " + this.dest.index + " " +
                this.cost + " " + this.cost_environmental + " " +this.limit);
    }
}