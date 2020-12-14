/**
 * clasa Graph defineste un graf orinetat
 *
 * Atributele sunt: un vector de noduri, numarul de noduri (size) si numarul maxim de drumui ale unui nod
 */
public class Graph {

    final private int MAX_ROAD_SIZE = 30;
    final private int DEFAULT_SIZE = 20;
    final private int size;
    final private Node[] nodes;

    public Graph() {
        this.size = DEFAULT_SIZE;
        this.nodes = new Node[this.size + 1];
        for (int i = 0; i < this.size; i++) {
            this.nodes[i] = new Node(MAX_ROAD_SIZE, i);
        }
    }

    public Graph(int size) {
        this.size = size;
        this.nodes = new Node[this.size + 1];
        for (int i = 0; i < this.size; i++) {
            this.nodes[i] = new Node(MAX_ROAD_SIZE, i);
        }
    }

    /**
     * used for debugging
     */
    public void print() {
        for (int i = 0; i < this.size; i++) {
            this.nodes[i].print();
        }
    }

    /**
     * adauga un drum nou la un nod
     *
     * @param start: indexul nodului de start al drumui
     * @param end: indexul nodului de sfarsit al drumui
     * @param cost: costul drumului
     * @param size: limita de gabarit a drumului
     */
    public void addStreet(int start, int end, int cost, int size) {
        this.nodes[start].addRoad(this.nodes[end], cost, size);
    }

    /**
     * creaza un obiect Restriction nou si il asociaza unui drum
     *
     * @param type: tipul ambuteiajului
     * @param start: indexul nodului de start al drumului
     * @param end: indexul nodului de sfarsit al drumului
     * @param cost: costul ambuteiajului
     */
    public void addRestriction(String type, int start, int end, int cost) {
        Node src = this.nodes[start];
        Restriction restr = new Restriction(type, cost);
        for (int i = 0; i < src.getSize(); i++) {
            if (src.getRoad(i).getDest() == end) {
                src.getRoad(i).restrict(restr);
            }
        }
    }

    /**
     * reface recursiv drumul cel mai scurt pentru afisare
     *
     * @param prev_index: vectorul de noduri precedente creat de alg Dijkstra
     * @param cur: indexul nodului curent
     * @param rez: un StringBuffer in care este adaugat (append) fiecare nod prin care trece drumul
     */
    private void backtrack(int[] prev_index, int cur, StringBuffer rez) {
        if (!(cur == prev_index[cur])) {
            backtrack(prev_index, prev_index[cur], rez);
        }
        rez.append("P" + cur + " ");
    }

    /**
     * foloseste alg Dijkstra pentru a calcula drumul cel mai scurt
     *
     * metoda foloseste un obiect PriorityQueue pentru a implementa algoritmul, si doi vectori:
     *  - unul in care retine costul drumului cel mai scurt pentru fiecare nod (costs)
     *  - unul in care retine indexul nodului precedent parcurs pentru fiecare nod (prev_index)
     *
     * @param veh: un obiect de tip vehicle care ofera informatiile necesare de cost si gabarit
     * @param start: nodul de inceput
     * @param end: nodul de final
     * @return path: un StringBuffer care contine atat fiecare nod parcurs, cat si costul total
     */
    public String drive(Vehicle veh, int start, int end) {
        int[] costs = new int[this.size + 1];
        int[] prev_index = new int[this.size + 1];
        for (int i = 0; i < this.size; i++)
            costs[i] = Integer.MAX_VALUE;
        costs[start] = 0;
        prev_index[start] = start;

        PriorityQueue q = new PriorityQueue(this.size);
        q.push(start, 0);

        while(!q.isEmpty()) {
            q.updateQueue();
            Node aux = this.nodes[q.pop()];
            for (int i = 0; i < aux.getSize(); i ++) {
                if (aux.getRoad(i).canDrive(veh)) {
                    int cur = costs[aux.index] + aux.getRoad(i).computeCost(veh);
                    if (costs[aux.getRoad(i).getDest()] > cur) {
                        q.push(aux.getRoad(i).getDest(), cur);
                        costs[aux.getRoad(i).getDest()] = cur;
                        prev_index[aux.getRoad(i).getDest()] = aux.index;
                    }
                }
            }
        }

        StringBuffer path = new StringBuffer();
        if (costs[end] == Integer.MAX_VALUE)
            path.append("P" + start + " " + "P" + end + " null");
        else {
            backtrack(prev_index, end, path);
            path.append(costs[end]);
        }
        return path.toString();
    }
}
