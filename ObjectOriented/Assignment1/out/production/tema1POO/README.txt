Buzatu-Pahontu Mihnea-Nicolae
324CB

Tema 1 POO

Modelare:
    clasa Vehicle mostenita de clasele Bicycle, Motorbike, Car si Truck
        retine atributele vehiculului, cost si gabarit
        atributele sunt setate de constructor in functie de clasa
        o metoda statica care creaza un vehicul in functie de tip

    clasa Restriction
        reprezita un ambuteiaj
        atributele sunt tipul si valoarea costului

    clasa Road
        reprezinta un drum intre doua noduri
        atributele sunt:
            - un nod sursa, constant
            - un nod destinatie, constant
            - costul, limita de gabarit si un cost total al ambuteiajelor
        metode de verificare a gabaritului, de calculare a costului total si de
        adaugare a unui ambuteiaj

    clasa Node
        reprezinta un nod al grafului
        atributele sunt:
            - indexul nodului, constant
            - un vector de drumuri care pleaca din nod
            - numarul de drumuri si numarul maxim de drumuri
            - o constanta pentru numarul maxim implicit de drumuri
        metode de adaugare a unui drum si de parcurge a vectorului de drumuri

    clasa Graph
        reprezinta un graf orientat
        atributele sunt:
            - numarul de noduri, constant
            - un vector de noduri cu lungime constanta
            - doua constante pentru numarul maxim de drumuri ale unui nod
              si numarul implicit de noduri pentru constructorul fara param
        metode:
            - adaugare de drum la un nod
            - adaugare de ambuteiaj la un drum oarecare al grafului
            - calculare a celui mai scurt drum folosind algoritmul lui
              Dijkstra implementat cu o coada de prioritati
                (drumul este intors ca un String format din nodurile prin care
                 trece si, la final, valoarea)
            - metoda privata backtrack care reconstruieste recursiv
              drumul cel mai scurt

    clasa PriorityQueue
        reprezita o coada de prioritati
        principiul de functionare este cel de minHeap, in functie de priority
        atributele sunt:
            - numarul maxim de elemente
            - numarul curent de elemente
            - un vector de elemente
            - un vector de prioritati
                (prioritatea lui elem[i] este priority[i])
        metode:
            - verificare daca coada este goala
            - verificare daca un element se afla in coada
            - pop si push(push verifica elementul se afla deja in coada, si daca
              da, ii updateaza prioritatea)
            - updateQueue() care restaureaza prorpietatea de minHeap, deoarece
              aceasta se pierde in urma operatiilor de push sau pop
                (as fi putut sa apelez aceasta metoda la finalul unui push sau pop,
                dar am preferat sa nu pentru a avea mai mult control asupra structurii)

        Am ales sa modelez aceasta structura folosind indexul nodurilor si nu
        nodurile in sine doarece mi sa parut mai simplu.

    clasa Main
        Deschide fisierele, citeste inputul linie cu linie si creaza graful
        apeleaza metoda drive si scrie in fisier


Astept feedback legat de modelare si de folosirea principiilor OOP.
Multumesc!


