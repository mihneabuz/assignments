/**
 * Implementeaza o structura de date de tip Priority Queue, folosind un minHeap
 *
 * Atributele sunt: un numar curent de elemente si un numar maxim de elemente,
 * un vector de elemente si un vector al prioritatilor, astfel incat
 * prioriatea lui elem[i] este priority[i]
 * Sunt extrase mai intai elementele cu priority minim
 */
public class PriorityQueue {
    final static private int DEFAULT_MAX_SIZE = 1000;
    final private int max_size;
    private int size;
    final private int[] elem;
    final private int[] priority;

    public PriorityQueue(int max_size) {
        this.max_size = max_size;
        this.size = 0;
        elem = new int[max_size + 1];
        priority = new int[max_size + 1];
    }

    public PriorityQueue() {
        this.max_size = DEFAULT_MAX_SIZE;
        this.size = 0;
        elem = new int[max_size + 1];
        priority = new int[max_size + 1];
    }

    private int parent(int pos) {
        return pos / 2;
    }

    /**
     * interschimba 2 elemente din heap
     *
     * @param pos1: primul element de intershimbat
     * @param pos2: al doilea element de intershimbat
     */
    private void swap(int pos1, int pos2)
    {
        int tmp;
        tmp = elem[pos1];
        elem[pos1] = elem[pos2];
        elem[pos2] = tmp;
        tmp = priority[pos1];
        priority[pos1] = priority[pos2];
        priority[pos2] = tmp;
    }

    /**
     * @return 1 daca coada e goala, 0 daca are elemente
     */
    public boolean isEmpty() {
        if (this.size == 0)
            return true;
        return false;
    }

    /**
     * schimba prioritatea unui element din coada
     *
     * @param elem: valoare elementului
     * @param value: noua valoare a prioritatii
     */
    private void updatePriority(int elem, int value) {
        for (int i = 1; i <= this.size; i++) {
            if (this.elem[i] == elem)
                this.priority[i] = value;
        }
    }

    /**
     * verifica daca coada contine un element
     *
     * @param elem: elementul cautat
     * @return 1 daca este gasit, si 0 altfel
     */
    public boolean hasElem(int elem) {
        for (int i = 1; i <= this.size; i++) {
            if (this.elem[i] == elem)
                return true;
        }
        return false;
    }

    /**
     * restaureaza proprietatea de minHeap a vectorului de prioritati
     *
     * metodele de adaugare si extragere a elementelor strica proprietatea de minHeap
     * a vectorilor si ordinea corecta a cozii. Aceasta metoda este necesara pentru a
     * restabilii ordinea corecta a cozii si pentru a asigura ca este extras elementul corect
     */
 public void updateQueue() {
        int k;
        for (int i = 2; i <= this.size; i++) {
            k = i;
            while (this.priority[k] < this.priority[parent(k)]) {
                this.swap(k, parent(k));
                k = parent(k);
            }
        }
    }

    /**
     * pentru debugging
     */
    public void print() {
        if (this.isEmpty())
            System.out.println("Queue is empty.");
        else
            for (int i = 1; i <= this.size; i ++) {
                System.out.println(this.priority[i]);
            }
    }

    /**
     * adauga un element nou la coada; daca elementul exita, schimba doar prioritatea
     *
     * @param elem: elementul ce trebuie adaugat
     * @param priority: prioritatea elementului
     */
    public void push(int elem, int priority) {
        if (this.hasElem(elem))
            this.updatePriority(elem, priority);
        else {
            this.size++;
            this.elem[this.size] = elem;
            this.priority[this.size] = priority;
        }
    }

    /**
     * extrage si returneaza primul element din coada (cu priority cel mai mic)
     *
     * @return elementul din varful cozii
     */
    public int pop() {
        if (this.isEmpty()) {
            System.out.println("Queue is empty.");
            return 0;
        }
        int popped = this.elem[1];
        this.swap(1, this.size);
        this.size--;
        return popped;
    }
}
