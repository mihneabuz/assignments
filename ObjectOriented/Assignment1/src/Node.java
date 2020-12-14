/**
 * clasa Node reprezinta un nod al grafului
 *
 * atributele sunt un index in graf, un vector de drumuri si numarul de drumuri
 */
class Node {
    final private int DEFAULT_MAX_SIZE = 20;
    final int index;
    final private Road[] roads;
    private int size;
    final private int max_size;

    public Node() {
        this.index = -1;
        this.max_size = DEFAULT_MAX_SIZE;
        this.size = 0;
        this.roads = new Road[this.max_size];
    }

    public Node(int max_size, int index) {
        this.index = index;
        this.max_size = max_size;
        this.size = 0;
        this.roads = new Road[this.max_size];
    }

    /**
     * intoarce drumul cu un anumit index, folosit pentru parcurgerea drumurilor
     *
     * @param index: indexul drumului dorit
     * @return drumul cu indexul index
     */
    public Road getRoad(int index) {
        if (index > this.size)
            return null;
        return this.roads[index];
    }

    public int getSize() {
        return this.size;
    }

    /**
     * folosit pentru debugging
     */
    public void print() {
        for (int i = 0; i < this.size; i++) {
            this.roads[i].print();
        }
    }

    /**
     * creaza un drum nou care incepe din nodul curent, si il adauga la vectorul de drumuri
     *
     * @param dest: nodul destinatie al drumului
     * @param cost: costul drumui
     * @param limit: limita de gabarit a drumului
     */
    public void addRoad(Node dest, int cost, int limit) {
        this.roads[size] = new Road(this, dest, cost, limit);
        this.size ++;
    }

}